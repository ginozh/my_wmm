package com.example.vpn

import android.content.Context
import android.content.Intent
import android.widget.Button
import android.widget.EditText
import android.widget.TextView
import android.os.AsyncTask
import android.os.Build
import android.os.Bundle
import androidx.annotation.RequiresApi
import androidx.appcompat.app.AppCompatActivity

import com.beust.klaxon.Klaxon
import com.beust.klaxon.JsonReader
import com.beust.klaxon.JsonObject
import java.io.IOException
import java.io.StringReader
import java.util.Base64
import javax.crypto.Cipher
import javax.crypto.spec.IvParameterSpec
import javax.crypto.spec.SecretKeySpec
import org.jsoup.Jsoup
import org.jsoup.Connection

class VpnRegisterActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_vpn_register)
        var et_user_name = findViewById(R.id.re_user_name) as EditText
        var et_password = findViewById(R.id.re_password) as EditText
        var et_again_password = findViewById(R.id.re_again_password) as EditText
        var btn_re_login = findViewById(R.id.re_login) as TextView
        var btn_register = findViewById(R.id.re_btn_register) as Button
        btn_re_login?.setOnClickListener {
            val intent = Intent(this, VpnLoginActivity::class.java).apply {
            }
            startActivity(intent)
        }
        btn_register?.setOnClickListener {
            var username = et_user_name.text.toString()
            var et_password = et_password.text.toString()
        }
    }
    inner class WebScratch : AsyncTask<Void, Void, Void>() {
        private lateinit var words: String

        @RequiresApi(Build.VERSION_CODES.O)
        override fun doInBackground(vararg params: Void): Void? {
            try {
                //currentTime=$(date +"%Y-%m-%d %H:%M:%S");curl -k   https://backend.mediaflyer.org/vpnserver/loginvpn/ -H "identity:solid_vpn_request"   -H "vpntoken: 1234567890" -H "machineid:keykeykey" -H "localtime:$currentTime" -H "address:127.0.0.1" -H "country:US" -H "os:Win" -H "email: hqzhang1983@gmail.com" -H "password: a1234567890"
                //currentTime=$(date +"%Y-%m-%d %H:%M:%S");curl -k   https://backend.mediaflyer.org/vpnserver/registervpn/ -H "identity:solid_vpn_request"   -H "vpntoken: 1234567890" -H "machineid:keykeykey" -H "localtime:$currentTime" -H "address:127.0.0.1" -H "country:US" -H "os:Win" -H "email: hqzhang1983@gmail.com" -H "password: a1234567890"
                val document =
                    Jsoup.connect("https://backend.mediaflyer.org/vpnserver/registervpn/")
                        .header("Identity", "vpn.com")
                        .timeout(160000).validateTLSCertificates(false)
                        .execute().body()
            } catch (e: IOException) {
                e.printStackTrace()
            }
            return null
        }
    }
}
