package com.example.vpn

//import com.example.vpn.ui.login.LoginActivity
//import android.R
//import android.R

import android.app.Activity
import android.content.Intent
import android.os.Build
import android.os.Bundle
import android.os.Environment
import android.text.Html
import android.widget.ImageView
import android.widget.RelativeLayout
import android.widget.TextView
import androidx.annotation.RequiresApi
import androidx.appcompat.app.AppCompatActivity
import com.beust.klaxon.Klaxon
import com.beust.klaxon.KlaxonException
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import java.text.SimpleDateFormat
import java.util.*


const val EXTRA_USER = "com.example.vpn.user"
const val EXTRA_PASSWD = "com.example.vpn.passwd"
const val EXTRA_EXPIRE = "com.example.vpn.expire"

open class VpnMainActivity : AppCompatActivity() {
    data class VPNServer(val Ip: String="", val Name: String="", val Country: String="", val Ovpn: String="")
    data class Server(val Servers: Array<VPNServer> = emptyArray())
    data class UserInfo(var email: String="", var password: String="", var expire_date: String="")
    data class VpnInfo(var vpnName: String="", var vpnCountry: String="", var lastdata: String="")
    private var userInfo=UserInfo()
    private var selectedVpnInfo=VpnInfo()
    private var vpnServers=Server()

    private val LOGIN_ACTIVITY_REQUEST_CODE = 0
    @RequiresApi(Build.VERSION_CODES.M)
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_vpn_main)
        //val btnOne = findViewById<Button>(R.id.btnLogin)
        val tvLogin = findViewById<TextView>(R.id.main_tv_login) as TextView
        val layoutConnect = findViewById<RelativeLayout>(R.id.main_layout_connect) as RelativeLayout
        val ivConnect = findViewById<ImageView>(R.id.main_iv_connect) as ImageView
        val tvConnect = findViewById<TextView>(R.id.main_tv_connect) as TextView
        val ivCountry = findViewById<ImageView>(R.id.main_iv_country) as ImageView
        val tvExpire = findViewById<TextView>(R.id.main_tv_expire) as TextView
        val ivBuy = findViewById<ImageView>(R.id.main_iv_buy) as ImageView
        val ivHelp = findViewById<ImageView>(R.id.main_iv_help) as ImageView

        // 1, 判断是否是中国用户
        // 1.1, 如果是直接退出
        CoroutineScope( Dispatchers.Main ).launch{
            println("VpnMainActivity current_thread: ${Thread.currentThread().name}")
            val country = suspendingGetCountry().trim() as String
            if (country.equals("CN",true)){
                println("VpnMainActivity exit") // TODO
            }
        }
        println("Environment getDataDirectory: " + Environment.getDataDirectory())
        //val fileDeal = FileSecurity()
        // 1, 读取本地用户名、密码数据
        // {"email": "111@gmail.com", "password": "a123456", "expire_date": "2020-10-23 05:14:59"}
        val userContent = FileSecurity.readData(applicationContext, g_user_director,  g_user_file) as String
        // 1.1, 如果有用户名、密码数据则登陆, 获取过期时间
        if(userContent.isNotEmpty()){
            println("userContent: $userContent")
            try {
                userInfo = Klaxon().parse<UserInfo>(userContent) as UserInfo
                tvLogin.text = Html.fromHtml("<b><u>${userInfo.email}</u></b>")
                tvExpire.text = userInfo.expire_date
                //tvLogin.setPaintFlags(tvLogin.paintFlags | Paint.UNDERLINE_TEXT_FLAG)
            }catch (e: KlaxonException) {
                e.printStackTrace()
            }
        }else{
            // 1.2, 如果没有用户名
            tvLogin.text = Html.fromHtml("<b><u>Please login</u></b>")
            //tvLogin.setPaintFlags(tvLogin.paintFlags | Paint.UNDERLINE_TEXT_FLAG);
            /*
            val userContentTmp=
            """ 
            {"email": "hqzhang1983@gmail.com", "password": "a1234567890", "expire_date": "2020-10-24 03:14:59"}
            """
            FileSecurity.writeData(applicationContext, g_user_director, g_user_file, userContentTmp);
            */
        }
        // return
        // 2, 读取本地vpn信息, 以及上次拉取的时间&&选中的vpnserver
        // {"vpnserver": "country_name", "lastdata": ""}
        val vpnInfoStr = FileSecurity.readData(applicationContext, g_vpn_director,  g_vpninfo_file) //as String
        // 2.1, 如果有
        if(vpnInfoStr.isNotEmpty()){
            try {
                selectedVpnInfo = Klaxon().parse<VpnInfo>(vpnInfoStr) as VpnInfo
            }catch (e: KlaxonException) {
                e.printStackTrace()
            }
            val vpnServersStr = FileSecurity.readData(applicationContext, g_vpn_director,  g_vpn_file) //as String
            if(vpnServersStr.isNotEmpty()){
                try {
                    vpnServers = Klaxon().parse<Server>(vpnServersStr) as Server
                }catch (e: KlaxonException) {
                    e.printStackTrace()
                }
            }
            println("vpnInfoStr: $vpnInfoStr");
        }
        // 2.2, 如果没有, 则拉取vpn数据数据
        if(vpnServers.Servers.isEmpty()){
            CoroutineScope( Dispatchers.Main ).launch{
                val vpnServersStr = suspendingGetVpnServers() //as String
                if (vpnServersStr.isEmpty()){
                    println("VpnMainActivity error") // TODO
                }else{
                    FileSecurity.writeData(applicationContext, g_vpn_director, g_vpn_file, vpnServersStr);
                    try {
                        vpnServers = Klaxon().parse<Server>(vpnServersStr) as Server
                    }catch (e: KlaxonException) {
                        e.printStackTrace()
                    }
                }
            }
        }
        if (selectedVpnInfo.vpnName.isEmpty() && vpnServers.Servers.isNotEmpty()) {
            selectedVpnInfo.vpnName = vpnServers.Servers[0].Name  // TODO 随机选择一个?
            selectedVpnInfo.vpnCountry = vpnServers.Servers[0].Country  
            selectedVpnInfo.lastdata = SimpleDateFormat( "yyyy-MM-dd HH:mm:ss", Locale.getDefault()).toString()
            println("VpnMainActivity vpnName: ${selectedVpnInfo.vpnName} vpnCountry: ${selectedVpnInfo.vpnCountry}")
            val country = selectedVpnInfo.vpnCountry.toLowerCase()
            ivCountry.setImageResource(resources.getIdentifier("country_$country","drawable","com.example.vpn"))
            FileSecurity.writeData(applicationContext, g_vpn_director, g_vpninfo_file, Klaxon().toJsonString(selectedVpnInfo));
            //ivCountry.setImageResource(R.drawable.circle_1)
        }

        val hasUser=false
        tvLogin.setOnClickListener {
            //val intent = Intent(this, LoginActivity::class.java).apply
            val intent = Intent(this, VpnLoginActivity::class.java).apply{
                putExtra(EXTRA_USER, userInfo.email)
                putExtra(EXTRA_PASSWD, userInfo.password)
            }
            //startActivity(intent)
            startActivityForResult(intent, LOGIN_ACTIVITY_REQUEST_CODE);
        }
        layoutConnect.setOnClickListener {
            println("connect")
            tvConnect.text = "disconnect"
            ivConnect.setImageResource(R.drawable.circle_1)
        }
        ivCountry.setOnClickListener {
            val intent = Intent(this, VpnLoginActivity::class.java).apply{
                if (hasUser){
                    putExtra(EXTRA_USER, "user")
                    putExtra(EXTRA_PASSWD, "passwd")
                }
            }
            startActivity(intent)
            //ivCountry.setImageResource(R.drawable.circle_1)
        }
        ivBuy.setOnClickListener {
            val intent = Intent(this, VpnLoginActivity::class.java).apply{
                if (hasUser){
                    putExtra(EXTRA_USER, "user")
                    putExtra(EXTRA_PASSWD, "passwd")
                }
            }
            startActivity(intent)
        }
        ivHelp.setOnClickListener {
            val intent = Intent(this, VpnLoginActivity::class.java).apply{
            }
            startActivity(intent)
        }
    }
    override  fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)

        // check that it is the SecondActivity with an OK result
        if (requestCode == LOGIN_ACTIVITY_REQUEST_CODE) {
            if (resultCode == Activity.RESULT_OK) {
                // get String data from Intent
                val email = data?.getStringExtra(EXTRA_USER).toString()
                val password = data?.getStringExtra(EXTRA_PASSWD).toString()// as String
                val expire_date = data?.getStringExtra(EXTRA_EXPIRE).toString()// as String
                if (userInfo.email != email || userInfo.password != password || userInfo.expire_date != expire_date){
                    userInfo.email = email
                    userInfo.password = password
                    userInfo.expire_date = expire_date
                    val tvLogin = findViewById<TextView>(R.id.main_tv_login) as TextView
                    val tvExpire = findViewById<TextView>(R.id.main_tv_expire) as TextView
                    tvLogin.text = Html.fromHtml("<b><u>${userInfo.email}</u></b>")
                    tvExpire.text = userInfo.expire_date
                    //val fileDeal = FileSecurity()
                    FileSecurity.writeData(applicationContext, g_user_director, g_user_file, Klaxon().toJsonString(userInfo));
                }
                println("login user: ${userInfo.email} passwd: ${userInfo.password} expire: ${userInfo.expire_date}")
            }
        }
    }
    /*
    fun getVpnServers(): Server{
        var r = Server()
        try {
            val document =
                Jsoup.connect("https://ips.mediaflyer.org:4430/centerserver")
                .header("Identity", "vpn.com")
                .ignoreContentType(true)
                .timeout(160000).validateTLSCertificates(false)
                .execute().body()
            println(document)
            try {
                r = Klaxon().parse<Server>(document) as Server
                with(r!!.Servers) {
                    for (i in 0..this.size-1)
                    {
                        println("ip: "+this[i].Ip)
                    }
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
            val r: Server
            r = getVpnServers()
            //words = r.message
            return null
        }
        override fun onPostExecute(aVoid: Void?) {
            super.onPostExecute(aVoid)
            //et_user_name.setText(words)
        }
    }*/
}
