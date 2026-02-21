package dev.blackoutburst.client.tile

import dev.blackoutburst.bogel.graphics.TextureArray
import org.lwjgl.opengl.GL15.glGenBuffers
import org.lwjgl.opengl.GL30.glGenVertexArrays

data class TileLayer(
    var order: Int,
    var visible: Boolean = true,
    val tiles: MutableList<Tile> = mutableListOf(),
    var glVAO: Int = glGenVertexArrays(),
    var glVBO: Int = glGenBuffers(),
    var glVertices: FloatArray = floatArrayOf(),
    val textureMap: MutableMap<String, Int> = mutableMapOf(),
    var textureSize: Int = 16,
    var diffuseMap: TextureArray? = null,
)