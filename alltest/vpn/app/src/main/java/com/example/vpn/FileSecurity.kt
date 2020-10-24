package com.example.vpn

import android.content.Context
import java.io.File
import javax.crypto.Cipher
import javax.crypto.spec.IvParameterSpec
import javax.crypto.spec.SecretKeySpec

const val g_user_director = "/u4/"
const val g_user_file = "u.data"
const val g_vpn_director = "/v/"
const val g_vpn_file = "v.data"
const val g_vpninfo_file = "vi.data"

internal object Foo {
    fun a(): Int {
        return 1
    }
}
internal object FileSecurity {
    fun init(){
    }
    private fun encDec(content: ByteArray, isEnc: Boolean): ByteArray{
        val aseKey = "mediflyer_aeskey".toByteArray(Charsets.UTF_8)
        var iv = ByteArray(16)
        val aesTransformation = "AES/CBC/PKCS5Padding"
        lateinit var keySpec:SecretKeySpec
        lateinit var ivSpec:IvParameterSpec
        val charArray = "mediflyer_aeskey11".toCharArray()
        println("iv.size: " + iv.size + " charArray.size: " + charArray.size)
        for (i in charArray.indices) {
            if (i >= iv.size) {
                println("great iv.size")
                break
            }
            iv[i] = charArray[i].toByte()
            //println("i: $i")
        }
        keySpec = SecretKeySpec(aseKey, "AES")
        ivSpec = IvParameterSpec(iv)
        val cipher = Cipher.getInstance(aesTransformation)
        if (isEnc){
            cipher.init(Cipher.ENCRYPT_MODE, keySpec, ivSpec)
        }else{
            cipher.init(Cipher.DECRYPT_MODE, keySpec, ivSpec)
        }
        return cipher.doFinal(content)
    }

    fun writeData(context: Context, directory: String, filename: String, content: String){
        try {
            // 1, file

            val finalDirectory = context.filesDir.toString() + directory
            val fileDirectory = File(finalDirectory)
            if(!fileDirectory.exists()) {
                fileDirectory.mkdirs()
            }
            //val finalDirectory = Environment.getDataDirectory()

            println("FileSecurity::writeData directory: $finalDirectory filename: $filename")
            val file = File(finalDirectory, filename) //as File
            // 2, enc
            val result = encDec(content.toByteArray(), true)
            println("FileSecurity::writeData content: $content size: " + content.length + "result:" + result + " size: " + result.size);
            // 3, write
            file.writeBytes(result);
        }catch (e: java.io.FileNotFoundException){
            e.printStackTrace()
        }
    }

    fun readData(context: Context, dir: String, filename: String): String{
        val directory = context.filesDir.toString() + dir
        //val directory = Environment.getDataDirectory()
        val file = File(directory, filename) //as File
        println("directory: $directory filename: $filename")
        // 1, judge exist
        if(!file.exists()) {
            return ""
        }
        // 2, openfile
        var content = file.readBytes()
        // 3, dec
        val decryptedByteValue = encDec(content, false)
        println("FileSecurity::readData content: $content size: " + content.size + "decryptedByteValue:" + String(decryptedByteValue) +" size: " + decryptedByteValue.size);
        return String(decryptedByteValue)
    }
}
