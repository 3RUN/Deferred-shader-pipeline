# Deferred pipeline for 3dgs engine

This is a deferred pipeline that includes shadow mapping for pointlights.
You can see it in motion on my YouTube channel:
https://youtu.be/8dLz9Z62TRo

It supports
- Shadow mapping up to 8 dynamic lights
- Level blocks
- Particles
- Translucent objects (fading)
- Fog
- Multitexture terrain (up to 16 terrains)
- PSone effects from RetroAcknex (vertex snapping, affine mapping, polygon cutoff)
- Output color posterization

It doesn't support
- Sunlight
- Spotlights

This pipeline originaly (back in 2012) was created by José Miguel Rodríguez Chávarri, aka txesmi.
Then in 2020 it was refactored and tweaked by both of us (txesmi and me).
