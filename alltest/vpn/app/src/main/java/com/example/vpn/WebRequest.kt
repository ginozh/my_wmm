package com.example.vpn

import android.text.Editable
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext
import org.jsoup.Jsoup
import java.io.IOException
import java.text.SimpleDateFormat
import java.util.*
// https://kaixue.io/kotlin-coroutines-1/
// https://developer.android.com/topic/libraries/architecture/coroutines#viewmodelscope
/*
suspend fun suspendingGetImage(id: String) = withContext(Dispatchers.IO){
    println("current_thread: ${Thread.currentThread().name}")
    return@withContext "test"
}
*/
suspend fun suspendingGetCountry() = withContext(Dispatchers.IO){
    var document:String = ""
    try {
        val uniqueID: String = UUID.randomUUID().toString()
        println("WebRequest::getLoginInfo uniqueID: $uniqueID")
        document =
            Jsoup.connect("https://backend.mediaflyer.org/get_ip/")
                .header("Identity", "solid_vpn_request")
                .ignoreContentType(true)
                .timeout(160000).validateTLSCertificates(false)
                .execute().body()
    } catch (e: IOException) {
        e.printStackTrace()
    }
    return@withContext document
}

suspend fun suspendingGetLoginInfo(email: Editable, password: Editable)= withContext(Dispatchers.IO){
    var document:String = ""
    val dateTimeFormat = SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault())
    try {
        //currentTime=$(date +"%Y-%m-%d %H:%M:%S");curl -k   https://backend.mediaflyer.org/vpnserver/loginvpn/ -H "identity:solid_vpn_request"   -H "vpntoken: 1234567890" -H "machineid:keykeykey" -H "localtime:$currentTime" -H "address:127.0.0.1" -H "country:US" -H "os:Win" -H "email: hqzhang1983@gmail.com" -H "password: a1234567890"
        val uniqueID: String = UUID.randomUUID().toString()
        println("WebRequest::getLoginInfo uniqueID: $uniqueID")
        document =
            Jsoup.connect("https://backend.mediaflyer.org/vpnserver/loginvpn/")
                .header("Identity", "solid_vpn_request")
                .header("vpntoken", "1234567890")
                .header("machineid", "keykeykey")
                //.header("localtime", "2020-10-19 10:35:36")
                .header("localtime", dateTimeFormat.toString())
                .header("address", "127.0.0.1")
                .header("country", "US")
                .header("os", "Android")
                .header("email", "hqzhang1983@gmail.com")
                .header("password", "a1234567890")
                .ignoreContentType(true)
                .timeout(160000).validateTLSCertificates(false)
                .execute().body()
    } catch (e: IOException) {
        e.printStackTrace()
    }
    return@withContext document
}

suspend fun suspendingRegister(email: String, password: String)= withContext(Dispatchers.IO){
    var document:String = ""
    val dateTimeFormat = SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault())
    try {
        //currentTime=$(date +"%Y-%m-%d %H:%M:%S");curl -k   https://backend.mediaflyer.org/vpnserver/registervpn/ -H "identity:solid_vpn_request"   -H "vpntoken: 1234567890" -H "machineid:keykeykey" -H "localtime:$currentTime" -H "address:127.0.0.1" -H "country:US" -H "os:Win" -H "email: hqzhang1983@gmail.com" -H "password: a1234567890"
        val uniqueID: String = UUID.randomUUID().toString()
        println("WebRequest::getLoginInfo uniqueID: $uniqueID")
        document =
            Jsoup.connect("https://backend.mediaflyer.org/vpnserver/registervpn/")
                .header("Identity", "solid_vpn_request")
                .header("vpntoken", "1234567890")
                .header("machineid", "keykeykey")
                .header("localtime", dateTimeFormat.toString())
                .header("address", "127.0.0.1")
                .header("country", "US")
                .header("os", "Android")
                .header("email", "hqzhang1983@gmail.com")
                .header("password", "a1234567890")
                .ignoreContentType(true)
                .timeout(160000).validateTLSCertificates(false)
                .execute().body()
    } catch (e: IOException) {
        e.printStackTrace()
    }
    return@withContext document
}

suspend fun suspendingGetVpnServers()= withContext(Dispatchers.IO){
    var document:String = ""
    try {
        document =
            Jsoup.connect("https://ips.mediaflyer.org:4430/centerserver")
            .header("Identity", "vpn.com")
            .ignoreContentType(true)
            .timeout(160000).validateTLSCertificates(false)
            .execute().body()
    } catch (e: IOException) {
        e.printStackTrace()
    }
    return@withContext document
}
