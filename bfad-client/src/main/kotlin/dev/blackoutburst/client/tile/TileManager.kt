package dev.blackoutburst.client.tile

import dev.blackoutburst.bogel.camera.Camera
import dev.blackoutburst.bogel.graphics.Texture
import dev.blackoutburst.bogel.graphics.TextureArray
import dev.blackoutburst.bogel.maths.Matrix
import dev.blackoutburst.bogel.shader.Shader
import dev.blackoutburst.bogel.shader.ShaderProgram
import dev.blackoutburst.bogel.utils.stack
import dev.blackoutburst.client.Main
import org.lwjgl.opengl.GL20.*
import org.lwjgl.opengl.GL30.GL_TEXTURE_2D_ARRAY
import org.lwjgl.opengl.GL30.glBindVertexArray

object TilesManager {
    private val missingTexture = Texture("textures/tiles/error.png")

    var layers = mutableListOf<TileLayer>()

    private val vertexShader = Shader(GL_VERTEX_SHADER, "/shaders/tiles.vert")
    private val fragmentShader = Shader(GL_FRAGMENT_SHADER, "/shaders/tiles.frag")
    private val shaderProgram = ShaderProgram(vertexShader, fragmentShader)

    private val model = Matrix()

    fun refreshDiffuseMap() {
        for (layer in layers) {
            layer.diffuseMap?.let { glDeleteTextures(it.id) }

            layer.textureMap.clear()

            Main.tileTextureFolder.forEachIndexed { index, file ->
                layer.textureMap[file] = index
            }

            layer.diffuseMap =
                TextureArray(
                    Main.tileTextureFolder.map { "textures/tiles/$it" },
                    size = layer.textureSize,
                    fromJar = true
                )

            generate(layer)
        }
    }

    private fun generate(layer: TileLayer) {
        val vertexArray = mutableListOf<Float>()

        for (t in layer.tiles) {
            vertexArray.addAll(listOf(layer.textureMap[t.texture]?.toFloat() ?: -1f, t.position.x, t.position.y, 0.0f, 0.0f, t.color.r, t.color.g, t.color.b, t.color.a))
            vertexArray.addAll(listOf(layer.textureMap[t.texture]?.toFloat() ?: -1f, t.position.x + t.size.x, t.position.y, 1.0f, 0.0f, t.color.r, t.color.g, t.color.b, t.color.a))
            vertexArray.addAll(listOf(layer.textureMap[t.texture]?.toFloat() ?: -1f, t.position.x + t.size.x, t.position.y + t.size.y, 1.0f, 1.0f, t.color.r, t.color.g, t.color.b, t.color.a))

            vertexArray.addAll(listOf(layer.textureMap[t.texture]?.toFloat() ?: -1f, t.position.x + t.size.x, t.position.y + t.size.y, 1.0f, 1.0f, t.color.r, t.color.g, t.color.b, t.color.a))
            vertexArray.addAll(listOf(layer.textureMap[t.texture]?.toFloat() ?: -1f, t.position.x, t.position.y + t.size.y, 0.0f, 1.0f, t.color.r, t.color.g, t.color.b, t.color.a))
            vertexArray.addAll(listOf(layer.textureMap[t.texture]?.toFloat() ?: -1f, t.position.x, t.position.y, 0.0f, 0.0f, t.color.r, t.color.g, t.color.b, t.color.a))
        }

        layer.glVertices = vertexArray.toFloatArray()

        stack(256 * 1024) { stack ->
            glBindVertexArray(layer.glVAO)

            // VBO
            glBindBuffer(GL_ARRAY_BUFFER, layer.glVBO)
            val vertexBuffer = stack.mallocFloat(layer.glVertices.size)
            vertexBuffer.put(layer.glVertices).flip()
            glBufferData(GL_ARRAY_BUFFER, vertexBuffer, GL_STATIC_DRAW)

            // ATTRIB
            // TEXTURE ID
            glEnableVertexAttribArray(0)
            glVertexAttribPointer(0, 1, GL_FLOAT, false, 36, 0)

            // POSITION
            glEnableVertexAttribArray(1)
            glVertexAttribPointer(1, 2, GL_FLOAT, false, 36, 4)

            // UV
            glEnableVertexAttribArray(2)
            glVertexAttribPointer(2, 2, GL_FLOAT, false, 36, 12)

            // COLOR
            glEnableVertexAttribArray(3)
            glVertexAttribPointer(3, 4, GL_FLOAT, false, 36, 20)
        }
    }

    fun addTile(layer: TileLayer, tile: Tile) {
        layer.tiles.add(tile)

        generate(layer)
    }

    fun render() {
        for (layer in layers) {
            glBindVertexArray(layer.glVAO)
            glUseProgram(shaderProgram.id)

            layer.diffuseMap?.let {
                glActiveTexture(GL_TEXTURE0)
                glBindTexture(GL_TEXTURE_2D_ARRAY, it.id)
                shaderProgram.setUniform1i("diffuseMap", 0)
            }

            glActiveTexture(GL_TEXTURE1)
            glBindTexture(GL_TEXTURE_2D, missingTexture.id)
            shaderProgram.setUniform1i("missingDiffuseMap", 1)
            shaderProgram.setUniformMat4("model", model)
            shaderProgram.setUniformMat4("projection", Camera.projection2D)
            shaderProgram.setUniformMat4("view", Camera.view)

            if (layer.visible)
                glDrawArrays(GL_TRIANGLES, 0, layer.glVertices.size / 9)
        }
    }
}