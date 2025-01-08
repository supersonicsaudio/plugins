# Supersonics Audio Plugins

Welcome to Supersonics Audio!<br><br>
Here you will find [code examples](code) of Supersonics Audio Plugins.
There are also links for [sound samples](https://www.youtube.com/@SupersonicsAudio) .
<br>
<p align="center">
 <img src="images/superreverbposter.png" width="800">
</p>
<p align="center">
 <img src="images/pluginsposter.png" width="500">
</p> 

## Plugins
* [Supersonics SuperReverb](#supersonics-superreverb)
* [WSOLAPitchShifter](#wsola-pitchshifter)
* [DynamicsShaper](#dynamicsshaper/gate)
* [TripleHead VintagePitchShifter](#tripleHead-VintagePitchShifter)

Tested in macOS intel (Sonoma 14.5), macOS m1 (Sonoma 14.5) and Intel Windows10 PC using Reaper as DAW.
#### Installation
* Download and extract the plugin into your plugin's folder

On Windows the VST3 folder is located in
```
C:\Program Files (x86)\Common Files\VST3
```
On MAC the AU folder is located in
```
/Library/Audio/Plug-Ins/Components 
```

* Keep in mind you may need to rescan the plugins folder in your DAW to detect the newly copied plugin in your plugins folder. 
* In some instances like Logic Pro, you may need to completely quit Logic Pro and restart it again in order for the software to update its plugins list.

## Supersonics SuperReverb
A plate-style reverb with shimmer control.

[Hear it in action](https://youtu.be/517ETR3p5vo).<br>
[![](https://img.youtube.com/vi/517ETR3p5vo/sddefault.jpg)](https://youtu.be/517ETR3p5vo) 
## WSOLA PitchShifter
A WSOLA-type pitch shifter. Preserves much of the orignal formants. Most efficient with monophonic signals like voice (speech) or single string instrument. 
Sounds good with polyphonics signals too.

[Hear it in action](https://youtu.be/-DjL_VXSGQc).<br>
[![](https://img.youtube.com/vi/-DjL_VXSGQc/0.jpg)](https://youtu.be/-DjL_VXSGQc) 
## DynamicsShaper/Gate 
A classic ADSR processor. The ADSR is triggered when the signal reachs a treshold. 
User can modify the attack and decay of the instrument to make it sound like another.
Source code [here](code/Supersonics-DynamicsShaper).

[Hear it in action](https://youtu.be/rb3IziiWplQ).<br>
[![](https://img.youtube.com/vi/rb3IziiWplQ/0.jpg)](https://youtu.be/rb3IziiWplQ) 

## TripleHead VintagePitchShifter
Pitch shifter effect that makes use of the overlap-add scheme using three delay lines. User can select between two or three playback heads, the direction of the playback and the size of the processing block.

Some cancellations does exist during the cross-fades. This makes the sound throbbing or tremolo-like so formants are not preserved.
It is also an interesting effect when abused.
Source code [here](code/Supersonics-TripleHead-PitchShifter).

[Hear it in action](https://youtu.be/suXz9YRy8k0).<br>
[![](https://img.youtube.com/vi/suXz9YRy8k0/0.jpg)](https://youtu.be/suXz9YRy8k0) 



### Hope you have fun and some outwordly sounds!
 
## Help

Should you have any further question or doubt dont heasitate to get in touch. We/I am actively available at 
```
supersonics@supersonicsaudio.com
```
Keep the air moving!<br>
Supersonics Audio
