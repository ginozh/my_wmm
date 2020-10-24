package com.example.vpn

import android.app.Activity
import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import com.beust.klaxon.Klaxon
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch

/*
interface OnEventListener<T> {
    fun onSuccess(tobject: T)
    fun onFailure(e: Exception?)
}
*/
data class LoginInfo(val error: Int, val message: String, val expire_date: String = "", val status: Int = 0, val payment_date: String = "", val free: Int = 0)

class VpnLoginActivity : AppCompatActivity() {
    private lateinit var et_user_name: EditText
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_vpn_login)
        /*
        CoroutineScope( Dispatchers.Main ).launch{
            println("current_thread: ${Thread.currentThread().name}")
            //val bitmap = suspendingGetImage("1")
            val bitmap = suspendingGetLoginInfo("", "")
            println("bitmap: $bitmap")
        }*/


        /*
        // it is ok
        val someTask = SomeTask(
            applicationContext,
            object : OnEventListener<String?> {

                override fun onFailure(e: java.lang.Exception?) {
                    Toast.makeText(
                        applicationContext,
                        "ERROR: " + e!!.message,
                        Toast.LENGTH_LONG
                    ).show()
                }

                override fun onSuccess(result: String?) {
                    // TODO("Not yet implemented")
                    println("SUCCESS $result")
                    Toast.makeText(applicationContext, "SUCCESS $result", Toast.LENGTH_LONG)
                        .show()
                }

            })
        someTask.execute()
        */
        //return
        // get reference to all views
        et_user_name = findViewById(R.id.et_user_name) as EditText
        var et_password = findViewById(R.id.et_password) as EditText
        //var btn_register = findViewById(R.id.btn_register) as Button
        var btn_register = findViewById(R.id.btn_register) as TextView
        //var btn_forget_passwd = findViewById(R.id.btn_forget_passwd) as Button
        var btn_login = findViewById(R.id.btn_login) as Button

        et_user_name.setText(intent.getStringExtra(EXTRA_USER))
        et_password.setText(intent.getStringExtra(EXTRA_PASSWD))
        //btn_register?.setPaintFlags(btn_register.getPaintFlags() or Paint.UNDERLINE_TEXT_FLAG)
        btn_register.setOnClickListener {
            val intent = Intent(this, VpnRegisterActivity::class.java).apply {
            }
            startActivity(intent)
        }
        btn_login?.setOnClickListener {
            // WebScratch().execute()
            // TODO 回调处理: https://stackoverflow.com/questions/9963691/android-asynctask-sending-callbacks-to-ui
            // TODO 成功返回主界面
            //      onBackPressed()
            if (et_user_name.text.isNotEmpty() && et_password.text.isNotEmpty()){
                CoroutineScope( Dispatchers.Main ).launch{
                    val userContent = suspendingGetLoginInfo(et_user_name.text, et_password.text)// as String
                    val r = Klaxon().parse<LoginInfo>(userContent) as LoginInfo
                    if (r.error == 0) {
                        val intent = Intent().apply{
                            putExtra(EXTRA_USER, et_user_name.text.toString())
                            putExtra(EXTRA_PASSWD, et_password.text.toString())
                            putExtra(EXTRA_EXPIRE, r.expire_date)
                        }
                        println("VpnLoginActivity user: ${et_user_name.text} passwd: ${et_password.text} expire: ${r.expire_date}")
                        setResult(Activity.RESULT_OK, intent)
                        finish()
                    }else{
                        // TODO show tips
                    }
                }
            }
        }
    }
    /*
    fun getLoginInfo(): LoginInfo{
        var r = LoginInfo(0, "")
        val dateTimeFormat = SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault())
        try {
            //currentTime=$(date +"%Y-%m-%d %H:%M:%S");curl -k   https://backend.mediaflyer.org/vpnserver/loginvpn/ -H "identity:solid_vpn_request"   -H "vpntoken: 1234567890" -H "machineid:keykeykey" -H "localtime:$currentTime" -H "address:127.0.0.1" -H "country:US" -H "os:Win" -H "email: hqzhang1983@gmail.com" -H "password: a1234567890"
            val document =
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
            var words = document as String //.text()
            println(words)
            // success: {"error": 0, "message": "ok", "expire_date": "2020-10-21 02:17:02", "status": 0, "payment_date": "None", "free": 0}
            // fail: {"error": -120, "message": "no user"}
            try {
                r = Klaxon().parse<LoginInfo>(words) as LoginInfo
                println("error: " + r.error)
                if (r.error != 0) {
                    words = r.message

                }
            }catch (e: KlaxonException) {
                e.printStackTrace()
            }

        } catch (e: IOException) {
            e.printStackTrace()
        }
        return r
    }

    inner class WebScratch : AsyncTask<Void, Void, Void>() {
        private lateinit var words: String

        @RequiresApi(Build.VERSION_CODES.O)
        override fun doInBackground(vararg params: Void): Void? {
            val r: LoginInfo
            r = getLoginInfo()
            words = r.message
            return null
        }
        override fun onPostExecute(aVoid: Void?) {
            super.onPostExecute(aVoid)
            et_user_name.setText(words)
        }
    }
    inner class SomeTask(context: Context, callback: OnEventListener<*>?) : AsyncTask<Void?, Void?, String?>() {
        private val mCallBack: OnEventListener<String?>?
        private val mContext: Context
        var mException: java.lang.Exception? = null
        @RequiresApi(Build.VERSION_CODES.O)
        protected override fun doInBackground(vararg p0: Void?): String? {
            mException = try {
                // todo try to do something dangerous
                println("doInBackground")
                return "HELLO"
            } catch (e: java.lang.Exception) {
                e
            }
            return null
        }

        override fun onPostExecute(result: String?) {
            println("onPostExecute")
            if (mCallBack != null) {
                if (mException == null) {
                    println("onPostExecute success")
                    mCallBack.onSuccess(result)
                } else {
                    println("onPostExecute fail")
                    mCallBack.onFailure(mException)
                }
            }
        }

        init {
            mCallBack = callback as OnEventListener<String?>?
            mContext = context
        }
    }
    */
}
