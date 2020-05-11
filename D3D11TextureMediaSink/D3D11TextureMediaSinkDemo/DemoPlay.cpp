#include "stdafx.h"
#include "../D3D11TextureMediaSink/D3D11TextureMediaSink.h"
#include "DemoPlay.h"

DemoPlay::DemoPlay(HWND hWnd)
{
	HRESULT hr = S_OK;
	m_refCount = 0;
	m_bInitialized = FALSE;
	m_hrStatus = S_OK;
	m_bPresentNow = false;
	m_pWork = ::CreateThreadpoolWork(DemoPlay::PresentSwapChainWork, this, NULL);
	m_evReadyOrFailed = ::CreateEvent(NULL, TRUE, FALSE, NULL);

	// Setting up MediaFoundation
	::MFStartup(MF_VERSION);

	// Creating ID3D11Device and IDXGISwapChain
	D3D_FEATURE_LEVEL featureLevels[] =	{ D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL feature_level;
	RECT clientRect;
	::GetClientRect(hWnd, &clientRect);
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	::ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferDesc.Width = clientRect.right - clientRect.left;
	swapChainDesc.BufferDesc.Height= clientRect.bottom - clientRect.top;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;
	hr = ::D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		featureLevels,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_pDXGISwapChain,
		&m_pD3DDevice,
		&feature_level,
		&m_pD3DDeviceContext);
	if (FAILED(hr))
		return;

	// Set multithreaded mode to ON. Required if dxva is used.
	ID3D10Multithread* pD3D10Multithread;
	do
	{
		if (FAILED(hr = m_pD3DDevice->QueryInterface(IID_ID3D10Multithread, (void**)&pD3D10Multithread)))
			break;

		pD3D10Multithread->SetMultithreadProtected(TRUE);

	} while (FALSE);
	SafeRelease(pD3D10Multithread);
	if (FAILED(hr))
		return;

	// Creating and Registering D3D Devices with IMFDXGIDeviceManager
	if (FAILED(hr = ::MFCreateDXGIDeviceManager(&m_DXGIDeviceManagerID, &m_pDXGIDeviceManager)))
		return;
	m_pDXGIDeviceManager->ResetDevice(m_pD3DDevice, m_DXGIDeviceManagerID);

	// get IDXGIOutput
	IDXGIDevice* pDXGIDevice;
	IDXGIAdapter* pDXGIAdapter;
	do
	{
		if (FAILED(hr = m_pD3DDevice->QueryInterface(IID_IDXGIDevice, (void**)&pDXGIDevice)))
			break;

		if (FAILED(hr = pDXGIDevice->GetAdapter(&pDXGIAdapter)))
			break;

		if (FAILED(hr = pDXGIAdapter->EnumOutputs(0, &m_pDXGIOutput)))
			break;
	
	} while (FALSE);
	SafeRelease(pDXGIAdapter);
	SafeRelease(pDXGIDevice);
	if (FAILED(hr))
		return;

	// Id for topology workqueue configuration. If you're going to be IUnknown, the content is fine.
	IMFMediaType* pID = NULL;
	::MFCreateMediaType(&pID);
	this->ID_RegistarTopologyWorkQueueWithMMCSS = (IUnknown*)pID;

	// Initialization complete
	m_bInitialized = TRUE;
}
DemoPlay::~DemoPlay()
{
}
void DemoPlay::Dispose()
{
	this->m_bInitialized = FALSE;

	//     Stop playing
	if (NULL != m_pMediaSession)
		m_pMediaSession->Stop();

	//     If it is showing, wait for it to finish.
	::WaitForThreadpoolWorkCallbacks(m_pWork, TRUE);
	::CloseThreadpoolWork(m_pWork);
	
	//     Releasing MediaFoundation
	SafeRelease(ID_RegistarTopologyWorkQueueWithMMCSS);
	SafeRelease(m_pMediaSinkAttributes);
	SafeRelease(m_pMediaSink);
	SafeRelease(m_pMediaSource);
	SafeRelease(m_pMediaSession);

	//     Release of DXGI, D3D11
	SafeRelease(m_pDXGIDeviceManager);
	SafeRelease(m_pDXGIOutput);
	SafeRelease(m_pDXGISwapChain);
	SafeRelease(m_pD3DDeviceContext);
	SafeRelease(m_pD3DDevice);

	//     Shutting Down MediaFoundation
	::MFShutdown();
}
HRESULT DemoPlay::Play(LPCTSTR szFile)
{
	HRESULT hr = S_OK;

	::ResetEvent(m_evReadyOrFailed);

	//     Create a MediaSession.
	if (FAILED(hr = this->CreateMediaSession(&m_pMediaSession)))
		return hr;

	//     Create a MediaSource from a file.
	if (FAILED(hr = this->CreateMediaSourceFromFile(szFile, &m_pMediaSource)))
		return hr;

	IMFTopology* pTopology = NULL;
	do
	{
		//         Create a partial topology.
		if (FAILED(hr = this->CreateTopology(&pTopology)))
			break;

		//         Register with MediaSession.
		if (FAILED(hr = m_pMediaSession->SetTopology(0, pTopology)))
			break;

	} while (FALSE);

	SafeRelease(pTopology);

	if (FAILED(hr))
		return hr;

	//     If MediaSession succeeds (or fails) to create a complete topology, the MESessionTopologyStatus event is issued and waits until then.
	::WaitForSingleObject(m_evReadyOrFailed, 5000);

	if (FAILED(m_hrStatus))
		return m_hrStatus;	//Failed to make

	//     Start playing MediaSession.
	PROPVARIANT prop;
	::PropVariantInit(&prop);
	m_pMediaSession->Start(NULL, &prop);
	
	return S_OK;
}
void DemoPlay::UpdateAndPresent()
{
	if (!m_bPresentNow )	// m_bPresentNow is atomic_bool
	{
		// Drawing process
		this->Draw();

		// SwapChain means
		m_bPresentNow = true;
		::SubmitThreadpoolWork(m_pWork);
	}
	else
	{
		//        Progress processing
		this->Update();
	}
}
void DemoPlay::Update()
{
	//    todo: What you want to do other than drawing
}
void DemoPlay::Draw()
{
	HRESULT hr = S_OK;

	IMFSample* pSample = NULL;
	IMFMediaBuffer* pMediaBuffer = NULL;
	IMFDXGIBuffer* pDXGIBuffer = NULL;
	ID3D11Texture2D* pTexture2D = NULL;
	ID3D11Texture2D* pBackBufferTexture2D = NULL;

	do
	{
		//        The IMFSample to display now is set to the TMS_SAMPLE attribute of TextureMediaSink, so get it.
		if (FAILED(hr = m_pMediaSinkAttributes->GetUnknown(TMS_SAMPLE, IID_IMFSample, (void**)&pSample)))
			break;
		if (NULL == pSample)
			break;	// un Settings

		// Get a media buffer from IMFSample.
		if (FAILED(hr = pSample->GetBufferByIndex(0, &pMediaBuffer)))
			break;

		// Get DXGI buffer from media buffer.
		if (FAILED(hr = pMediaBuffer->QueryInterface(IID_IMFDXGIBuffer, (void**)&pDXGIBuffer)))
			break;

		// Gets the source Texture2D from the DXGI buffer.
		if (FAILED(hr = pDXGIBuffer->GetResource(IID_ID3D11Texture2D, (void**)&pTexture2D)))
			break;

		// Gets the subresource index of the source Texture2D.
		UINT subIndex;
		if (FAILED(hr = pDXGIBuffer->GetSubresourceIndex(&subIndex)))
			break;

		//
		//You can now get id3D11Texture2D from IMFSample.
        //In this demo, we'll simply draw this rectangle on SwapChain.
		//

		// Get the destination Texture2D from SwapChain.
		if (FAILED(hr = m_pDXGISwapChain->GetBuffer(0, IID_ID3D11Texture2D, (void**)&pBackBufferTexture2D)))
			break;

		// Transfer
		m_pD3DDeviceContext->CopySubresourceRegion(pBackBufferTexture2D, 0, 0, 0, 0, pTexture2D, subIndex, NULL);

	} while (FALSE);

    // When you get a TMS_SAMPLE in GetUnknown, the IMFSample is locked so that it is not updated from TextureMediaSink.
    // To release this lock, setUnknwon something (anything) in the TMS_SAMPLE attribute. This allows IMFSample to be updated from TextureMediaSink.
	m_pMediaSinkAttributes->SetUnknown(TMS_SAMPLE, NULL);

	SafeRelease(pBackBufferTexture2D);
	SafeRelease(pTexture2D);
	SafeRelease(pDXGIBuffer);
	SafeRelease(pMediaBuffer);
	SafeRelease(pSample);
}

HRESULT DemoPlay::CreateMediaSession(IMFMediaSession** ppMediaSession)
{
	HRESULT hr = S_OK;

	IMFMediaSession* pMediaSession = nullptr;
	do
	{
		// Generate mediasession.
		if (FAILED(hr = ::MFCreateMediaSession(NULL, &pMediaSession)))
			break;

		// Start sending events from MediaSession.
		if (FAILED(hr = pMediaSession->BeginGetEvent(this, nullptr)))
			break;

		// Creation complete.
		(*ppMediaSession) = pMediaSession;
		(*ppMediaSession)->AddRef();
		hr = S_OK;

	} while (FALSE);

	SafeRelease(pMediaSession);

	return hr;
}
HRESULT DemoPlay::CreateMediaSourceFromFile(LPCTSTR szFile, IMFMediaSource** ppMediaSource)
{
	HRESULT hr = S_OK;

	IMFSourceResolver* pResolver = nullptr;
	IMFMediaSource* pMediaSource = nullptr;
	do
	{
		// Create a source resolver.
		if (FAILED(hr = ::MFCreateSourceResolver(&pResolver)))
			break;

		// Create a media source from the file path.
		MF_OBJECT_TYPE type;
		if (FAILED(hr = pResolver->CreateObjectFromURL(szFile, MF_RESOLUTION_MEDIASOURCE, NULL, &type, (IUnknown**)&pMediaSource)))
			break;

		// Creation complete.
		(*ppMediaSource) = pMediaSource;
		(*ppMediaSource)->AddRef();
		hr = S_OK;

	} while (FALSE);

	SafeRelease(pMediaSource);
	SafeRelease(pResolver);

	return hr;
}
HRESULT DemoPlay::CreateTopology(IMFTopology** ppTopology)
{
	HRESULT hr = S_OK;

	IMFTopology* pTopology = NULL;
	IMFPresentationDescriptor* pPresentationDesc = NULL;
	do
	{
		// Create a new topology.
		if (FAILED(hr = ::MFCreateTopology(&pTopology)))
			break;

		// Create a presentation identifier for the media source.
		if(FAILED(hr = m_pMediaSource->CreatePresentationDescriptor(&pPresentationDesc)))
			break;

		// Gets the number of streams in the media source from the presentation identifier.
		DWORD dwDescCount;
		if (FAILED(hr = pPresentationDesc->GetStreamDescriptorCount(&dwDescCount)))
			break;

		// For each stream of the media source, create a topology node and add it to the topology.
		for (DWORD i = 0; i < dwDescCount; i++)
		{
			if (FAILED(hr = this->AddTopologyNodes(pTopology, pPresentationDesc, i)))
				break;
		}
		if (FAILED(hr))
			break;

		// Creation complete.
		(*ppTopology) = pTopology;
		(*ppTopology)->AddRef();
		hr = S_OK;

	} while (FALSE);

	SafeRelease(pPresentationDesc);
	SafeRelease(pTopology);

	return hr;
}
HRESULT DemoPlay::AddTopologyNodes(IMFTopology* pTopology, IMFPresentationDescriptor* pPresentationDesc, DWORD index)
{
	HRESULT hr = S_OK;

	BOOL bSelected;
	IMFStreamDescriptor* pStreamDesc = NULL;
	IMFTopologyNode* pSourceNode = NULL;
	IMFTopologyNode* pOutputNode = NULL;
	do
	{
		// Gets the stream descriptor for the specified stream number.
		if (FAILED(hr = pPresentationDesc->GetStreamDescriptorByIndex(index, &bSelected, &pStreamDesc)))
			break;

		if (bSelected)
		{
			// (A) If a stream is selected, add it to the topology
			if (FAILED(hr = this->CreateSourceNode(pPresentationDesc, pStreamDesc, &pSourceNode)))
				break;

			GUID majorType;
			if (FAILED(hr = this->CreateOutputNode(pStreamDesc, &pOutputNode, &majorType)))
				break;

			if (majorType == MFMediaType_Audio)
			{
				pSourceNode->SetString(MF_TOPONODE_WORKQUEUE_MMCSS_CLASS, _T("Audio"));
				pSourceNode->SetUINT32(MF_TOPONODE_WORKQUEUE_ID, 1);
			}
			if (majorType == MFMediaType_Video)
			{
				pSourceNode->SetString(MF_TOPONODE_WORKQUEUE_MMCSS_CLASS, _T("Playback"));
				pSourceNode->SetUINT32(MF_TOPONODE_WORKQUEUE_ID, 2);
			}

			if (NULL != pSourceNode && NULL != pOutputNode)
			{
				pTopology->AddNode(pSourceNode);
				pTopology->AddNode(pOutputNode);

				pSourceNode->ConnectOutput(0, pOutputNode, 0);
			}
		}
		else
		{
			//  (B) If the stream is not selected, do nothing.
		}

	} while (FALSE);

	SafeRelease(pOutputNode);
	SafeRelease(pSourceNode);
	SafeRelease(pStreamDesc);

	return hr;
}
HRESULT DemoPlay::CreateSourceNode(IMFPresentationDescriptor* pPresentationDesc, IMFStreamDescriptor* pStreamDesc, IMFTopologyNode** ppSourceNode)
{
	HRESULT hr = S_OK;

	IMFTopologyNode* pSourceNode = NULL;
	do
	{
		// Create a source node.
		if (FAILED(hr = ::MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE, &pSourceNode)))
			break;

		// Set the source node with three attributes.
		if (FAILED(hr = pSourceNode->SetUnknown(MF_TOPONODE_SOURCE, m_pMediaSource)))
			break;
		if (FAILED(hr = pSourceNode->SetUnknown(MF_TOPONODE_PRESENTATION_DESCRIPTOR, pPresentationDesc)))
			break;
		if (FAILED(hr = pSourceNode->SetUnknown(MF_TOPONODE_STREAM_DESCRIPTOR, pStreamDesc)))
			break;
	
		// Creation complete.
		(*ppSourceNode) = pSourceNode;
		(*ppSourceNode)->AddRef();
		hr = S_OK;

	} while (FALSE);

	SafeRelease(pSourceNode);

	return hr;
}
HRESULT DemoPlay::CreateOutputNode(IMFStreamDescriptor* pStreamDesc, IMFTopologyNode** ppOutputNode, GUID* pMajorType)
{
	HRESULT hr = S_OK;

	IMFTopologyNode* pOutputNode = NULL;
	IMFMediaTypeHandler* pMediaTypeHandler = NULL;
	do
	{
		// Created an output node.
		if (FAILED(hr = ::MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE, &pOutputNode)))
			break;

		// Get media handler.
		if (FAILED(hr = pStreamDesc->GetMediaTypeHandler(&pMediaTypeHandler)))
			break;

		// Get measure type.
		GUID majorType;
		if (FAILED(hr = pMediaTypeHandler->GetMajorType(&majorType)))
			break;

		if (majorType == MFMediaType_Video)
		{
			//   (A) Video Renderer

			if (NULL == m_pMediaSink)
			{
				// Create TextureMediaSink.
				if (FAILED(hr = ::CreateD3D11TextureMediaSink(IID_IMFMediaSink, (void**)&m_pMediaSink, m_pDXGIDeviceManager, m_pD3DDevice)))
					break;

				// Get IMFAttributes for IMFSample receipt.
				if (FAILED(hr = m_pMediaSink->QueryInterface(IID_IMFAttributes, (void**)&m_pMediaSinkAttributes)))
					break;
			}

			IMFStreamSink* pStreamSink = NULL;
			do
			{
				// Gets the stream sink #0.
				if (FAILED(hr = m_pMediaSink->GetStreamSinkById(0, &pStreamSink)))
					break;

                //     Registerstream sink with output node.
				if (FAILED(hr = pOutputNode->SetObject(pStreamSink)))
					break;

			} while (FALSE);

			SafeRelease(pStreamDesc);
		}
		else if (majorType == MFMediaType_Audio)
		{
			// (B) Audio renderer

			IMFActivate* pActivate = NULL;
			do
			{
				// Generates default audio renderer activation.
				if (FAILED(hr = ::MFCreateAudioRendererActivate(&pActivate)))
					break;

				// Registered with the output node.
				if (FAILED(hr = pOutputNode->SetObject(pActivate)))
					break;

			} while (FALSE);

			SafeRelease(pActivate);
		}

		// Creation complete.
		(*ppOutputNode) = pOutputNode;
		(*ppOutputNode)->AddRef();
		hr = S_OK;

	} while (FALSE);

	SafeRelease(pMediaTypeHandler);
	SafeRelease(pOutputNode);

	return hr;
}

// IUnknown implementation
ULONG	DemoPlay::AddRef()
{
	return InterlockedIncrement(&this->m_refCount);
}
HRESULT DemoPlay::QueryInterface(REFIID iid, __RPC__deref_out _Result_nullonfailure_ void** ppv)
{
	if (NULL == ppv)
		return E_POINTER;

	if (iid == IID_IUnknown)
	{
		*ppv = static_cast<IUnknown*>(static_cast<IMFAsyncCallback*>(this));
	}
	else if (iid == __uuidof(IMFAsyncCallback))
	{
		*ppv = static_cast<IMFAsyncCallback*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	this->AddRef();

	return S_OK;
}
ULONG	DemoPlay::Release()
{
	ULONG uCount = InterlockedDecrement(&this->m_refCount);

	if (uCount == 0)
		delete this;

	return uCount;
}

// IMFAsyncCallback implementation
STDMETHODIMP DemoPlay::GetParameters(__RPC__out DWORD *pdwFlags, __RPC__out DWORD *pdwQueue)
{
	//     The implementation of this method is optional.
	return E_NOTIMPL;
}
STDMETHODIMP DemoPlay::Invoke(__RPC__in_opt IMFAsyncResult *pAsyncResult)
{
	if (!m_bInitialized)
		return MF_E_SHUTDOWN;

	HRESULT hr = S_OK;

	IUnknown* pUnk;
	if (SUCCEEDED(hr = pAsyncResult->GetState(&pUnk)))
	{
		//         (A) If you have state (not an error E_POINTER)

		if (this->ID_RegistarTopologyWorkQueueWithMMCSS == pUnk)
		{
			this->OnEndRegistarTopologyWorkQueueWithMMCSS(pAsyncResult);
			return S_OK;
		}
		else
		{
			return E_INVALIDARG;
		}
	}
	else
	{
		//         (B) If there is no State

		IMFMediaEvent* pMediaEvent = NULL;
		MediaEventType eventType;
		do
		{
			// Receive events from MediaSession.
			if (FAILED(hr = m_pMediaSession->EndGetEvent(pAsyncResult, &pMediaEvent)))
				break;
			if (FAILED(hr = pMediaEvent->GetType(&eventType)))
				break;
			if (FAILED(hr = pMediaEvent->GetStatus(&m_hrStatus)))
				break;

			// If the result fails, it is terminated.
			if (FAILED(m_hrStatus))
			{
				::SetEvent(m_evReadyOrFailed);
				return m_hrStatus;
			}

			// Branch by event type.
			switch (eventType)
			{
			case MESessionTopologyStatus:

				// Status get.
				UINT32 topoStatus;
				if (FAILED(hr = pMediaEvent->GetUINT32(MF_EVENT_TOPOLOGY_STATUS, &topoStatus)))
					break;
				switch (topoStatus)
				{
				case MF_TOPOSTATUS_READY:
					this->OnTopologyReady(pMediaEvent);
					break;
				}
				break;

			case MESessionStarted:
				this->OnSessionStarted(pMediaEvent);
				break;

			case MESessionPaused:
				this->OnSessionPaused(pMediaEvent);
				break;

			case MESessionStopped:
				this->OnSessionStopped(pMediaEvent);
				break;

			case MESessionClosed:
				this->OnSessionClosed(pMediaEvent);
				break;

			case MEEndOfPresentation:
				this->OnPresentationEnded(pMediaEvent);
				break;
			}

		} while (FALSE);

		SafeRelease(pMediaEvent);

		//         Wait for the next MediaSession event to be received.
		if (eventType != MESessionClosed)
			hr = m_pMediaSession->BeginGetEvent(this, NULL);

		return hr;
	}
}

void DemoPlay::OnTopologyReady(IMFMediaEvent* pMediaEvent)
{
	HRESULT hr;

	// Start class assignment (asynchronous processing) of the topology workqueue to MMCSS.

	IMFGetService* pGetService = NULL;
	IMFWorkQueueServices* pWorkQueueServices = NULL;
	do
	{
		if (FAILED(hr = this->m_pMediaSession->QueryInterface(IID_IMFGetService, (void**)&pGetService)))
			break;

		if (FAILED(hr = pGetService->GetService(MF_WORKQUEUE_SERVICES, IID_IMFWorkQueueServices, (void**)&pWorkQueueServices)))
			break;

		if (FAILED(hr = pWorkQueueServices->BeginRegisterTopologyWorkQueuesWithMMCSS(this, this->ID_RegistarTopologyWorkQueueWithMMCSS)))
			break;

	} while (FALSE);

	SafeRelease(pWorkQueueServices);
	SafeRelease(pGetService);

	if (FAILED(hr))
	{
		this->m_hrStatus = hr;
		::SetEvent(this->m_evReadyOrFailed);
	}
}
void DemoPlay::OnSessionStarted(IMFMediaEvent* pMediaEvent)
{
}
void DemoPlay::OnSessionPaused(IMFMediaEvent* pMediaEvent)
{
}
void DemoPlay::OnSessionStopped(IMFMediaEvent* pMediaEvent)
{
}
void DemoPlay::OnSessionClosed(IMFMediaEvent* pMediaEvent)
{
}
void DemoPlay::OnPresentationEnded(IMFMediaEvent* pMediaEvent)
{
}
void DemoPlay::OnEndRegistarTopologyWorkQueueWithMMCSS(IMFAsyncResult* pAsyncResult)
{
	HRESULT hr;

	//Complete the class assignment (asynchronous processing) of the topology workqueue to MMCSS.

	IMFGetService* pGetService = NULL;
	IMFWorkQueueServices* pWorkQueueServices = NULL;
	do
	{
		if (FAILED(hr = this->m_pMediaSession->QueryInterface(IID_IMFGetService, (void**)&pGetService)))
			break;

		if (FAILED(hr = pGetService->GetService(MF_WORKQUEUE_SERVICES, IID_IMFWorkQueueServices, (void**)&pWorkQueueServices)))
			break;

		if (FAILED(hr = pWorkQueueServices->EndRegisterTopologyWorkQueuesWithMMCSS(pAsyncResult)))
			break;

	} while (FALSE);

	SafeRelease(pWorkQueueServices);
	SafeRelease(pGetService);


	if (FAILED(hr))
		this->m_hrStatus = hr;
	else
		this->m_hrStatus = S_OK;

	::SetEvent(this->m_evReadyOrFailed);	// Event ignite.
}

// Tasks to view SwapChain
void CALLBACK DemoPlay::PresentSwapChainWork(PTP_CALLBACK_INSTANCE pInstance, LPVOID pvParam, PTP_WORK pWork)
{
	auto pDemoPlay = (DemoPlay*)pvParam;

	pDemoPlay->m_pDXGIOutput->WaitForVBlank();
	pDemoPlay->m_pDXGISwapChain->Present(1, 0);

	pDemoPlay->m_bPresentNow = false;
}

