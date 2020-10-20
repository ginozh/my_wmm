package com.example.vpn

//import com.example.vpn.ui.login.LoginActivity
import android.content.Intent
import android.os.AsyncTask
import android.os.Build
import android.os.Bundle
import android.widget.Button
import android.widget.ImageView
import android.widget.RelativeLayout
import android.widget.TextView
import androidx.annotation.RequiresApi
import androidx.appcompat.app.AppCompatActivity
import com.beust.klaxon.Klaxon
import com.beust.klaxon.KlaxonException
import org.jsoup.Jsoup
import org.jsoup.nodes.Document
import java.io.IOException

const val EXTRA_USER = "com.example.vpn.user"
const val EXTRA_PASSWD = "com.example.vpn.passwd"

class VpnMainActivity : AppCompatActivity() {
    data class VPNServer(val Ip: String="", val Name: String="", val Country: String="", val Ovpn: String="")
    data class Server(val Servers: Array<VPNServer> = emptyArray())
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_vpn_main)

        //val btnOne = findViewById<Button>(R.id.btnLogin)
        val tvlogin = findViewById<TextView>(R.id.main_btnLogin) as TextView
        val layoutconnect = findViewById<RelativeLayout>(R.id.main_layout_connect) as RelativeLayout
        val ivconnect = findViewById<ImageView>(R.id.main_iv_connect) as ImageView
        val tvconnect = findViewById<TextView>(R.id.main_tv_connect) as TextView
        val ivcountry = findViewById<ImageView>(R.id.main_iv_country) as ImageView
        val ivbuy = findViewById<ImageView>(R.id.main_iv_buy) as ImageView
        val ivhelp = findViewById<ImageView>(R.id.main_iv_help) as ImageView
        val hasUser = true
        // 1, 读取本地用户名、密码数据
        // TODO
        // 1.1, 如果有用户名、密码数据则登陆, 获取过期时间
        // TODO
        // 1.2, 如果没有用户名
        // TODO tvlogin.text = "login"
        // 2, 读取本地vpn信息, 上次选中的vpnserver
        val hasVpns = false
        // TODO
        // 2.1, 如果有,则设置国家
        // TODO
        // 2.2, 如果没有, 则拉取vpn数据数据
        if (!hasVpns){
            // WebScratch().execute()
        }
        tvlogin.setOnClickListener {
            //val intent = Intent(this, LoginActivity::class.java).apply
            val intent = Intent(this, VpnLoginActivity::class.java).apply{
                if (hasUser){
                    putExtra(EXTRA_USER, "user")
                    putExtra(EXTRA_PASSWD, "passwd")
                }
            }
            startActivity(intent)
        }
        layoutconnect.setOnClickListener {
            println("connect")
            tvconnect.setText("disconnect")
            ivconnect.setImageResource(R.drawable.circle_1)
        }
        ivcountry.setOnClickListener {
            val intent = Intent(this, VpnLoginActivity::class.java).apply{
                if (hasUser){
                    putExtra(EXTRA_USER, "user")
                    putExtra(EXTRA_PASSWD, "passwd")
                }
            }
            startActivity(intent)
            //ivcountry.setImageResource(R.drawable.circle_1)
        }
        ivbuy.setOnClickListener {
            val intent = Intent(this, VpnLoginActivity::class.java).apply{
                if (hasUser){
                    putExtra(EXTRA_USER, "user")
                    putExtra(EXTRA_PASSWD, "passwd")
                }
            }
            startActivity(intent)
        }
        ivhelp.setOnClickListener {
            val intent = Intent(this, VpnLoginActivity::class.java).apply{
                if (hasUser){
                    putExtra(EXTRA_USER, "user")
                    putExtra(EXTRA_PASSWD, "passwd")
                }
            }
            startActivity(intent)
        }
    }
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
    }
}
