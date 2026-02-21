package dev.blackoutburst.client.utils

import java.io.FileNotFoundException
import java.io.InputStream
import java.net.URL

class ResourceFile(private val path: String) {
    val url: URL
        get() = ResourceFile::class.java.getResource(path) ?: throw FileNotFoundException("$path not found")
    val inputStream: InputStream
        get() = ResourceFile::class.java.getResourceAsStream(path) ?: throw FileNotFoundException("$path not found")
}