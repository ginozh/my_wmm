package com.example.vpn

import android.app.Activity
import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.view.inputmethod.InputMethodManager
import android.widget.Button
import android.widget.EditText
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import com.beust.klaxon.Klaxon
import com.google.android.material.snackbar.Snackbar
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
data class ForgetPasswdInfo(val error: Int, val message: String)

class VpnLoginActivity : AppCompatActivity() {
    private lateinit var etUserName: EditText
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
        /*val callback = object : OnBackPressedCallback(
            true // default to enabled
        ) {
            override fun handleOnBackPressed() {
//your custom logic for back handling goes here
                println("VpnLoginActivity handleOnBackPressed")
            }
        }
        onBackPressedDispatcher.addCallback(
            this, // LifecycleOwner
            callback
        )*/
        //return
        // get reference to all views
        etUserName = findViewById<EditText>(R.id.login_et_user_name)
        var etPassword = findViewById<EditText>(R.id.login_et_password)
        //var tvRegister = findViewById<TextView>(R.id.login_tv_register)
        var tvForgetPassword = findViewById<TextView>(R.id.login_tv_forget_passwd)
        var btnLogin = findViewById<Button>(R.id.login_btn_login)

        etUserName.setText(intent.getStringExtra(EXTRA_USER))
        etPassword.setText(intent.getStringExtra(EXTRA_PASSWD))
        val country = intent.getStringExtra(EXTRA_COUNTRY).toString()
        //tvRegister?.setPaintFlags(tvRegister.getPaintFlags() or Paint.UNDERLINE_TEXT_FLAG)
        /*tvRegister.setOnClickListener {
            val intent = Intent(this, VpnRegisterActivity::class.java).apply {
            }
            startActivity(intent)
        }*/
        tvForgetPassword.setOnClickListener {
            val imm  = it.context.getSystemService(Context.INPUT_METHOD_SERVICE) as InputMethodManager
            imm.hideSoftInputFromWindow(it.windowToken, 0)

            tvForgetPassword.isClickable = false
            Snackbar.make(it, "Wait a minute...", Snackbar.LENGTH_LONG).setAction("Action", null).show()
            CoroutineScope( Dispatchers.Main ).launch{
                println("VpnMainActivity current_thread: ${Thread.currentThread().name}")
                // {"error": 0, "message": "sucess."}
                // {"error": -120, "message": "no user."}
                val forgetPasswordContent = suspendingForgetPassword(etUserName.text.toString(), country)
                val r = Klaxon().parse<ForgetPasswdInfo>(forgetPasswordContent) as ForgetPasswdInfo
                if (r.error == 0) {
                    Snackbar.make(it, "Change password link is sent to your email address", Snackbar.LENGTH_LONG)
                    .setAction("Action", null).show()
                }else{
                    Snackbar.make(it, "Sorry, This user does not exist", Snackbar.LENGTH_LONG)
                    .setAction("Action", null).show()
                }
                tvForgetPassword.isClickable = true
            }
        }
        btnLogin.setOnClickListener {
            // WebScratch().execute()
            // TODO 回调处理: https://stackoverflow.com/questions/9963691/android-asynctask-sending-callbacks-to-ui
            // TODO 成功返回主界面
            //      onBackPressed()
            val imm  = it.context.getSystemService(Context.INPUT_METHOD_SERVICE) as InputMethodManager
            imm.hideSoftInputFromWindow(it.windowToken, 0)
            if (etUserName.text.isNotEmpty() && etPassword.text.isNotEmpty()){
                btnLogin.isClickable = false
                Snackbar.make(it, "Wait a minute...", Snackbar.LENGTH_LONG).setAction("Action", null).show()
                CoroutineScope( Dispatchers.Main ).launch{
                    val userContent = suspendingGetLoginInfo(etUserName.text.toString(), etPassword.text.toString(), country)// as String
                    val r = Klaxon().parse<LoginInfo>(userContent) as LoginInfo
                    if (r.error == 0) {
                        val intent = Intent().apply{
                            putExtra(EXTRA_USER, etUserName.text.toString())
                            putExtra(EXTRA_PASSWD, etPassword.text.toString())
                            putExtra(EXTRA_EXPIRE, r.expire_date)
                        }
                        println("VpnLoginActivity user: ${etUserName.text} passwd: ${etPassword.text} expire: ${r.expire_date}")
                        setResult(Activity.RESULT_OK, intent)
                        finish()
                    }else{
                        // TODO show tips
                        Snackbar.make(it, "Incorrect password or user does not exist", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show()
                    }
                    btnLogin.isClickable = true
                }
            }

        }
    }
    /*override fun onOptionsItemSelected(item: MenuItem): Boolean {
        val id: Int = item.itemId
        println("VpnLoginActivity $id home: ${android.R.id.home}")
        if (id == android.R.id.home) {
            //NavUtils.navigateUpFromSameTask(this);
            val intent = Intent().apply{}
            setResult(Activity.RESULT_OK, intent)
            finish()
        }
        return super.onOptionsItemSelected(item)
    }*/
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
            etUserName.setText(words)
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
