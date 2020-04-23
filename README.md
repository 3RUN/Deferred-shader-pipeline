# Deferred pipeline for 3dgs engine

This is a deferred pipeline that includes shadow mapping for pointlights.
You can see it in motion on my YouTube channel:
https://youtu.be/Uco-yQ7-TCc

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

This pipeline originaly was created by José Miguel Rodríguez Chávarri, aka txesmi.
It was refactored, tweaked and changed by (ME) Magomet Kochkarov, aka 3RUN.
And statified by José Miguel Rodríguez Chávarri, aka txesmi.