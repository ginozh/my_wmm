#pragma once

namespace D3D11TextureMediaSink
{
	//IMarker:
	//IMFStreamSink: :P a custom interface for handling laceMarker calls asynchronously.
	////
	//The marker is
	//Marker type (MFSTREAMSINK_MARKER_TYPE)
	//Marker data (PROVARIANT)
	//Context (PROVARIANT)
	//It is composed of.
	////
	//This interface allows marker data to be stored inside the IUnknown object.
	//The object can be kept in the same queue as the queue that holds the media type.
	//This is useful, because samples and markers must be serialized.
	//In other words, you can't be responsible for markers until you've processed all the samples that came before them.
	////
	//Note that IMarker is not a standard Media Foundation interface.
	//
	MIDL_INTERFACE("3AC82233-933C-43a9-AF3D-ADC94EABF406")
	IMarker : public IUnknown
	{
		virtual STDMETHODIMP GetType(MFSTREAMSINK_MARKER_TYPE* pType) = 0;
		virtual STDMETHODIMP GetValue(PROPVARIANT* pvar) = 0;
		virtual STDMETHODIMP GetContext(PROPVARIANT* pvar) = 0;
	};
}
