# About this repo

This is a synthesizer. Inspired by [Syntorial](http://www.syntorial.com/) and i wish to build a plugins that easy and free to use.

Since this is my first attempt to make a vsti, it may works very slow.

# Special Thanks

 - Thanks [this tutorial](http://www.martin-finke.de/blog/tags/making_audio_plugins.html), this is a really great tutorial for developer who want to make an audio plugin with wdl-ol.
 - Thanks [wdl-ol](https://github.com/olilarkin/wdl-ol) because this repo is based on this.
 - Thanks [Knobman](http://www.g200kg.com/en/software/knobman.html) for makes creating knobs easily.
 - Thanks [SAVIHost](http://www.hermannseib.com/english/savihost.htm) for makes testing vsti easily.
 - Thanks [@UN1C0DE](https://github.com/JimmyBenKlieve/) for help me debugging and thanks [@Chris](https://github.com/chirs241097) for the advices.

# How to build

You need to clone [wdl-ol](https://github.com/olilarkin/wdl-ol) (VS2015 branch) to your disk, then clone this repo to /IPlugExamples/ folder, then use visual studio 2015 to open the Visual Studio .sln, then you can build.

If you want to build a vst2 or vst3 target, you should download the sdk and follow the instruction in `"wdl-ol/XXXX_SDK/readme.txt"` to place some header file to the right place.

I've also prepared the Code::Block project (.cbp) for user who want to build with that IDE. no Mac support right now, because i don't have Mac.

# Licence

MIT =D

# 关于这个Repo

想写合成器有很久了，于是经常有搜一些资料来看，偶然间看到了[这个教程](http://www.martin-finke.de/blog/tags/making_audio_plugins.html)，感觉甚好于是就跟着做了，目前根着教程然后做些符合我期望的改动，基本可用之后，我的合成器坑大概就要正式开工了。（大概....吧）

目前这个合成器还是非常简单的合成器，截至目前（[commit 0905d43](https://github.com/BLumia/PineappleSynth/commit/0905d434cc40460617b8f185c1ec489a983d55ae)）的功能基本根据上面提到的教程并稍微动点脑子写一点代码就可以达到。后续复杂的功能大概得慢慢啃DSP了。（先把简单功能完善了再说233）

还是非常感谢[那个教程](http://www.martin-finke.de/blog/tags/making_audio_plugins.html)把我拉入了DSP坑，当然我也把[@UN1C0DE](https://github.com/JimmyBenKlieve/)拉[入了坑](https://github.com/JimmyBenKlieve/ChipRhythm)。蛤蛤蛤蛤蛤蛤

受到了[Syntorial](http://www.syntorial.com/)的影响，PineappleSynth的初衷（开坑前设想）是做一个合成器和一系列教程，供需要和现在的我一样的小白点技能树使用。于是计划内会有基于 Web Audio API 的网页版和一个程序版共两个版本的大菠萝合成器出现。并具有类似 Syntorial 的帮助学习功能。

目前出于试水探坑底，于是现阶段会先做一个稳定可用的普通合成器，而不是有很多复杂功能的合成器。

# 感谢

 - 拜 [此教程](http://www.martin-finke.de/blog/tags/making_audio_plugins.html) 所赐，没有这个教程，大概这个repo也不会存在了吧。
 - 感谢 [wdl-ol](https://github.com/olilarkin/wdl-ol) ，整个项目基于此项目，虽然我做了一些微小的工作使之符合我的需求，这些改动待完善后也会放出。
 - [Knobman](http://www.g200kg.com/en/software/knobman.html) 是个做旋钮的不错的小工具，非常好用。
 - 调试使用了 [SAVIHost](http://www.hermannseib.com/english/savihost.htm) 作为vst2和vst3的宿主程序。
 - 额外感谢 [@UN1C0DE](https://github.com/JimmyBenKlieve/) 帮我抓虫子和 [@Chris](https://github.com/chirs241097) 的好点子 :D 

# 如何构建项目

你需要先去克隆 [wdl-ol](https://github.com/olilarkin/wdl-ol) (VS2015 分支) 到你的硬盘，然后把这个repo克隆到/IPlugExamples/文件夹，最后用VS2015打开里面的sln即可。

如果你想要构建vst2或者vst3，在你构建之前，你需要去准备好VST的SDK，然后根据 `"wdl-ol/XXXX_SDK/readme.txt"` 的引导放置需要的头文件即可。

我额外准备了code::block的cbp供这个IDE的用户编译需要的版本。需要注意的是，目前只能编译vst2版和app版。Mac AU版本暂不提供，因为我没苹果电脑。

# 许可协议

MIT~
