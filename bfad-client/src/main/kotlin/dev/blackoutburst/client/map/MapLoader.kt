package dev.blackoutburst.client.map

import dev.blackoutburst.bogel.maths.Vector2f
import dev.blackoutburst.bogel.utils.Color
import dev.blackoutburst.client.tile.Tile
import dev.blackoutburst.client.tile.TileLayer
import dev.blackoutburst.client.tile.TilesManager
import dev.blackoutburst.client.utils.ResourceFile
import java.io.BufferedReader
import java.io.InputStreamReader

object MapLoader {

    fun loadMap(mapName: String) {
        TilesManager.layers.clear()

        var layerIndex = 0
        val stream = ResourceFile("/map/$mapName").inputStream
        val reader = BufferedReader(InputStreamReader(stream))
        var line: String?

        while ((reader.readLine().also { line = it }) != null) {
            val split = line!!.split("|")
            if (line.startsWith("L")) {
                val layer = TileLayer(
                    order = layerIndex,
                    textureSize = split[3].toInt(),
                    visible = split[8].toBoolean(),
                )
                TilesManager.layers.add(layer)
                layerIndex++
            }
            if (line.startsWith("T")) {
                TilesManager.layers[split[1].toInt()].let {
                    TilesManager.addTile(
                        it,
                        Tile(
                            layer = split[1].toInt(),
                            texture = split[2],
                            position = Vector2f(split[3].toFloat(), split[4].toFloat()),
                            size = Vector2f(split[5].toFloat(), split[6].toFloat()),
                            color = Color(
                                split[7].toFloat(),
                                split[8].toFloat(),
                                split[9].toFloat(),
                                split[10].toFloat()
                            )
                        )
                    )
                }
            }
        }

        TilesManager.refreshDiffuseMap()
    }
}