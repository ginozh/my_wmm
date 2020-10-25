package com.example.vpn

import android.app.Activity
import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.util.Patterns
import android.view.View
import android.view.View.OnFocusChangeListener
import android.view.inputmethod.InputMethodManager
import android.widget.Button
import android.widget.EditText
import androidx.appcompat.app.AppCompatActivity
import com.beust.klaxon.Klaxon
import com.google.android.material.snackbar.Snackbar
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch

/*
class MyFocusChangeListener : OnFocusChangeListener {
    override fun onFocusChange(v: View, hasFocus: Boolean) {
        val imm  = v.context.getSystemService(Context.INPUT_METHOD_SERVICE) as InputMethodManager
        //if (v.id === R.id.register_et_user && !hasFocus)
        if (!hasFocus){
            imm.hideSoftInputFromWindow(v.windowToken, 0)
        }else{
            imm.showSoftInput(v, InputMethodManager.SHOW_IMPLICIT)
        }
    }
}
*/

class VpnRegisterActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_vpn_register)
        var etUserName = findViewById<EditText>(R.id.register_et_user)
        var etPassword = findViewById<EditText>(R.id.register_et_password)
        var etAgainPassword = findViewById<EditText>(R.id.register_et_re_password)
        //var btn_re_login = findViewById(R.id.re_login) as TextView
        var btnRegister = findViewById<Button>(R.id.register_btn_register)

        etUserName.setText(intent.getStringExtra(EXTRA_USER))
        etPassword.setText(intent.getStringExtra(EXTRA_PASSWD))
        val country = intent.getStringExtra(EXTRA_COUNTRY).toString()
        /*
        val ofcListener: OnFocusChangeListener = MyFocusChangeListener()
        etUserName.onFocusChangeListener = ofcListener
        etPassword.onFocusChangeListener = ofcListener
        etAgainPassword.onFocusChangeListener = ofcListener
        */
        /*btn_re_login.setOnClickListener {
            val intent = Intent(this, VpnLoginActivity::class.java).apply {
            }
            startActivity(intent)
        }*/
        btnRegister.setOnClickListener {
            /*etUserName.clearFocus()
            etPassword.clearFocus()
            etAgainPassword.clearFocus()*/
            val imm  = it.context.getSystemService(Context.INPUT_METHOD_SERVICE) as InputMethodManager
            imm.hideSoftInputFromWindow(it.windowToken, 0)

            var userName = etUserName.text.toString()
            var password = etPassword.text.toString()
            var againPassword = etAgainPassword.text.toString()
            if (userName.contains('@') && Patterns.EMAIL_ADDRESS.matcher(userName).matches()
                && password.length > 5 && password == againPassword) {
                it.isClickable = false
                Snackbar.make(it, "Wait a minute...", Snackbar.LENGTH_LONG) .setAction("Action", null).show()
                CoroutineScope(Dispatchers.Main).launch {
                    println("VpnMainActivity current_thread: ${Thread.currentThread().name}")
                    data class RegisterInfo(val error: Int, val message: String, val email: String, val expire_date: String = "", val payment_date: String = "", val free: Int = 0)
                    // {"error": 0, "email": "hqzhang1982@gmail.com", "message": "save successfully.", "expire_date": "2020-10-25 08:50:38", "payment_date": "None", "free": 1}
                    // {"error": -110, "message": "email was registered."}
                    val registerContent = suspendingRegister(userName, password, country)
                    val r = Klaxon().parse<RegisterInfo>(registerContent) as RegisterInfo
                    if (r.error == 0) {
                        val intent = Intent().apply{
                            putExtra(EXTRA_USER, etUserName.text.toString())
                            putExtra(EXTRA_PASSWD, etPassword.text.toString())
                            putExtra(EXTRA_EXPIRE, r.expire_date)
                        }
                        println("VpnRegisterActivity user: ${etUserName.text} passwd: ${etPassword.text} expire: ${r.expire_date}")
                        setResult(Activity.RESULT_OK, intent)
                        finish()
                    } else {
                        Snackbar.make( it, "This email was registered,please login with it ", Snackbar.LENGTH_LONG)
                            .setAction("Action", null).show()
                    }
                    it.isClickable = true
                }
            }else{
                if (!userName.contains('@') || !Patterns.EMAIL_ADDRESS.matcher(userName).matches()){
                    Snackbar.make( it, "This email is wrong", Snackbar.LENGTH_LONG)
                    .setAction("Action", null).show()
                }else if(password.length <= 5){
                    Snackbar.make( it, "This password isn't great 5", Snackbar.LENGTH_LONG)
                    .setAction("Action", null).show()
                }else if(password != againPassword) {
                    Snackbar.make( it, "Sorry, passwords do not match.", Snackbar.LENGTH_LONG)
                    .setAction("Action", null).show()
                }
            }
        }
    }
    /*
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
    */
}
