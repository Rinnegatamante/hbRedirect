# hbRedirect
An user plugin that redirects ux0 usage to other partitions. The use case of this plugin is about allowing to move data files of homebrew game/ports to secondary partitions mounted, most of the time, as uma0 or imc0.<br>
The idea came in mind when I've noticed there was a decent amount of interest towards my game ports in having support for other partitions for data files storing.

# How to Install
- Install [ioPlus](https://github.com/CelesteBlue-dev/PSVita-RE-tools/blob/master/ioPlus/ioPlus-0.1/release/ioplus.skprx) by downloading this file, placing it in your `tai` folder (generally in ur0 or ux0) and adding the plugin path under `*KERNEL` in your config.txt file.
- Download this plugin and place it as well in your `tai` folder.
- In your `config.txt` file, add a line for this plugin under `*ALL` section.
- Create the folder `ux0:data/hbRedirect` and place inside it a file with the title ID of the homebrew you want to use this plugin on with txt extension. (eg. `ux0:data/hbRedirect/GTAVCECTY.txt`)
- Write inside this file the partition you want to use for that specific homebrew (eg. `uma0`).

Here you can find a sample of how your config file should look like:
```
*KERNEL
ux0:tai/ioplus.skprx
*ALL
ux0:tai/hbRedirect.suprx
*main
ur0:tai/henkaku.suprx
*NPXS10015
ur0:tai/henkaku.suprx
*NPXS10016
ur0:tai/henkaku.suprx
```

# Notes
<b>This plugin is an entry for the [KyûHEN PSVITA homebrew contest](https://kyuhen.customprotocol.com/en/).</b>

# Credits
Special thanks to the distinguished patroners for their awesome support:
- @Sarkies_Proxy
- Badmanwazzy37
- Colin VanBuren
- drd7of14
- Freddy Parra
- Max
- Tain Sueiras
- The Vita3K Project
- Titi Clash
