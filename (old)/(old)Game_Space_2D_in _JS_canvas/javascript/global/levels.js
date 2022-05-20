//levels

function level_1(){
	spaceship.akturoven=1;
	spaceship.x=500;
	spaceship.y=350;
	spaceship.vx=0;
	spaceship.vy=0;
	spaceship.otocenie=0;
	spaceship.maxzivoty=700+300*vylepsenie[0];
	spaceship.maxstit=300+200*vylepsenie[7];
	spaceship.pohon=0.4+0.1*vylepsenie[1];
	spaceship.turbo=0.45+0.15*vylepsenie[8];
	spaceship.cd1=0;
	spaceship.cd2=0;
	pauza=1;
	ktory=0;
	ktora=0;
	asteroidy=[];
	naboje=[];
	vlna=1;
	enemiesplus=[8,0,0,0,0,0,0,0,0,0];
	spaceship.zivoty=spaceship.maxzivoty;
	spaceship.stit=spaceship.maxstit;
	for (i = 0; i < 2; i++)
		asteroidy.push(asteroid());
	spaceship.naraz=100;
	pozadie.texture=15;
	if(hudbaon==1)
	{
		hudba.pause();
		hudba = new Audio("music/Shipwreck_Trauma_by_Roman_Gerhat.mp3");
		hudba.volume=0.8;
		hudba.play();
		hudba.loop = true;
	}
	pomocna=2;
}

function level_2(){
	spaceship.akturoven=2;
	spaceship.x=500;
	spaceship.y=350;
	spaceship.vx=0;
	spaceship.vy=0;
	spaceship.otocenie=0;
	spaceship.maxzivoty=700+300*vylepsenie[0];
	spaceship.maxstit=300+200*vylepsenie[7];
	spaceship.pohon=0.4+0.1*vylepsenie[1];
	spaceship.turbo=0.45+0.15*vylepsenie[8];
	spaceship.cd1=0;
	spaceship.cd2=0;
	pauza=1;
	ktory=0;
	ktora=0;
	asteroidy=[];
	naboje=[];
	vlna=1;
	enemiesplus=[5,2,0,0,0,0,0,0,0,0];
	spaceship.zivoty=spaceship.maxzivoty;
	spaceship.stit=spaceship.maxstit;
	for (i = 0; i < 5; i++)
		asteroidy.push(asteroid());
	spaceship.naraz=100;
	pozadie.texture=57;
	if(hudbaon==1)
	{
		hudba.pause();
		hudba = new Audio("music/Distant_Aurora_by_Roman_Gerhat.mp3");
		hudba.volume=0.8;
		hudba.play();
		hudba.loop = true;
	}
	pomocna=2;
}

function level_3(){
	spaceship.akturoven=3;
	spaceship.x=500;
	spaceship.y=350;
	spaceship.vx=0;
	spaceship.vy=0;
	spaceship.otocenie=0;
	spaceship.maxzivoty=700+300*vylepsenie[0];
	spaceship.maxstit=300+200*vylepsenie[7];
	spaceship.pohon=0.4+0.1*vylepsenie[1];
	spaceship.turbo=0.45+0.15*vylepsenie[8];
	spaceship.cd1=0;
	spaceship.cd2=0;
	pauza=1;
	ktory=0;
	ktora=0;
	asteroidy=[];
	naboje=[];
	vlna=2;
	enemiesplus=[10,2,0,0,0,0,0,0,0,0];
	spaceship.zivoty=spaceship.maxzivoty;
	spaceship.stit=spaceship.maxstit;
	for (i = 0; i < 4; i++)
		asteroidy.push(asteroid());
	asteroidy.push(venom());
	asteroidy.push(venom());
	spaceship.naraz=100;
	pozadie.texture=58;
	if(hudbaon==1)
	{
		hudba.pause();
		hudba = new Audio("music/Floathing_by_Roman_Gerhat.mp3");
		hudba.volume=0.8;
		hudba.play();
		hudba.loop = true;
	}
	pomocna=2;
}

function level_4(){
	spaceship.akturoven=4;
	spaceship.x=500;
	spaceship.y=350;
	spaceship.vx=0;
	spaceship.vy=0;
	spaceship.otocenie=0;
	spaceship.maxzivoty=700+300*vylepsenie[0];
	spaceship.maxstit=300+200*vylepsenie[7];
	spaceship.pohon=0.4+0.1*vylepsenie[1];
	spaceship.turbo=0.45+0.15*vylepsenie[8];
	spaceship.cd1=0;
	spaceship.cd2=0;
	pauza=1;
	ktory=0;
	ktora=0;
	asteroidy=[];
	naboje=[];
	vlna=1;
	enemiesplus=[0,0,0,0,0,0,0,0,0,0];
	spaceship.zivoty=spaceship.maxzivoty;
	spaceship.stit=spaceship.maxstit;
	for (i = 0; i < 2; i++)
		asteroidy.push(abom());
	spaceship.naraz=100;
	pozadie.texture=80;
	if(hudbaon==1)
	{
		hudba.pause();
		hudba = new Audio("music/Distant_Aurora_by_Roman_Gerhat.mp3");
		hudba.volume=0.8;
		hudba.play();
		hudba.loop = true;
	}
	pomocna=2;
}

function level_5(){
	spaceship.akturoven=5;
	spaceship.x=500;
	spaceship.y=350;
	spaceship.vx=0;
	spaceship.vy=0;
	spaceship.otocenie=0;
	spaceship.maxzivoty=700+300*vylepsenie[0];
	spaceship.maxstit=300+200*vylepsenie[7];
	spaceship.pohon=0.4+0.1*vylepsenie[1];
	spaceship.turbo=0.45+0.15*vylepsenie[8];
	spaceship.cd1=0;
	spaceship.cd2=0;
	pauza=1;
	ktory=0;
	ktora=0;
	asteroidy=[];
	naboje=[];
	vlna=2;
	enemiesplus=[3,0,0,1,0,3,1,0,1,0];
	spaceship.zivoty=spaceship.maxzivoty;
	spaceship.stit=spaceship.maxstit;
	for (i = 0; i < 5; i++)
		asteroidy.push(asteroid());
	asteroidy.push(abom());
	asteroidy.push(venom());
	
	spaceship.naraz=100;
	pozadie.texture=83;
	if(hudbaon==1)
	{
		hudba.pause();
		hudba = new Audio("music/Distant_Aurora_by_Roman_Gerhat.mp3");
		hudba.volume=0.8;
		hudba.play();
		hudba.loop = true;
	}
	pomocna=2;
}

function level_6(){
	spaceship.akturoven=6;
	spaceship.x=500;
	spaceship.y=350;
	spaceship.vx=0;
	spaceship.vy=0;
	spaceship.otocenie=0;
	spaceship.maxzivoty=700+300*vylepsenie[0];
	spaceship.maxstit=300+200*vylepsenie[7];
	spaceship.pohon=0.4+0.1*vylepsenie[1];
	spaceship.turbo=0.45+0.15*vylepsenie[8];
	spaceship.cd1=0;
	spaceship.cd2=0;
	pauza=1;
	ktory=0;
	ktora=0;
	asteroidy=[];
	naboje=[];
	vlna=3;
	enemiesplus=[10,4,0,2,0,10,0,1,0,2];
	spaceship.zivoty=spaceship.maxzivoty;
	spaceship.stit=spaceship.maxstit;
	for (i = 0; i < 4; i++)
		asteroidy.push(asteroid());
	asteroidy.push(veteran());
	asteroidy.push(venom());
	asteroidy.push(krystal());
	spaceship.naraz=100;
	pozadie.texture=91;
	if(hudbaon==1)
	{
		hudba.pause();
		hudba = new Audio("music/Distant_Aurora_by_Roman_Gerhat.mp3");
		hudba.volume=0.8;
		hudba.play();
		hudba.loop = true;
	}
	pomocna=2;

}

function level_7(){
	spaceship.akturoven=7;
	spaceship.x=200;
	spaceship.y=600;
	spaceship.vx=0;
	spaceship.vy=0;
	spaceship.otocenie=45;
	spaceship.maxzivoty=700+300*vylepsenie[0];
	spaceship.maxstit=300+200*vylepsenie[7];
	spaceship.pohon=0.4+0.1*vylepsenie[1];
	spaceship.turbo=0.45+0.15*vylepsenie[8];
	spaceship.cd1=0;
	spaceship.cd2=0;
	pauza=1;
	ktory=0;
	ktora=0;
	asteroidy=[];
	naboje=[];
	vlna=3;
	enemiesplus=[0,0,0,0,0,0,0,0,0,0];
	spaceship.zivoty=spaceship.maxzivoty;
	spaceship.stit=spaceship.maxstit;
	asteroidy.push(vortex());
	spaceship.naraz=100;
	pozadie.texture=104;
	if(hudbaon==1)
	{
		hudba.pause();
		hudba = new Audio("music/Floathing_by_Roman_Gerhat.mp3");
		hudba.volume=0.8;
		hudba.play();
		hudba.loop = true;
	}
	pomocna=2;
}