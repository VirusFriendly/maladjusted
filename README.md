# Maladjusted

This was a project I worked on in 2003.

The original CVS Repo can be retrieved using the following command

 rsync -ai a.cvs.sourceforge.net::cvsroot/dmca/ .

At the core, this tool manipulated a elf32 binary so that it isn't
recognizable by libBFD utilities such as gdb, objdump, etc.

Back in the day IDApro also had issues identifying a maladjusted binary.

Sadly, current day IDApro can identify the elf for what it is.

Luckily, libBFD tools are still in the dark!

Shoutout to #MD2600 & Lawless

## Future Plans

I still have some tricks up my sleave and I wanted to see what I can do.
Sometime in the future I'll return to this repo and continue the research.
