//VORTEX
function vortex(){
	var naboj ={}
	naboj.velkostx=300;
	naboj.velkosty=300;
	naboj.x = 750;
	naboj.y = -50;
	naboj.texture = 59;
	naboj.otocenie = Math.random()*360;
	naboj.zivoty = 20000;
	naboj.maxzivoty = 0; //dmg naboju
	naboj.stit = 0;
	naboj.ktory = ktory++;
	naboj.timer = 300;
	naboj.timer_asteroid=100;
	naboj.timer_venom=1000;
	naboj.timer_abom=4000;
	naboj.timer_krystal=3000;
	naboj.timer_veteran=2000;
	naboj.special = function(){
	};
	
	naboj.move=  function() {
		this.velkostx=150+this.zivoty/100;
		this.velkosty=this.velkostx;
		this.x=1050-this.velkostx/2-175;
		this.y=300-this.velkostx/2-175;
		//ma zivoty?
		if(this.zivoty<=0)
		{
			spaceship.track.pause();
			setTimeout(function(){pauza=4;spaceship.score+=3000;}, 2000);
			pomocna=1;
			spaceship.score+=75000;
			delete(asteroidy[this.ktory]);
			if(spaceship.uroven<spaceship.akturoven)
				spaceship.uroven=spaceship.akturoven;
		}
		
		//priťahovanie spaceship
		if(this.x+this.velkostx/2>spaceship.x+spaceship.velkostx/2 && spaceship.vx<2 && this.timer < 100)
			spaceship.vx+=(spaceship.x+100)/7500;
		if(this.y+this.velkosty/2<spaceship.y+spaceship.velkosty/2 && spaceship.vy>-2 && this.timer < 100)
			spaceship.vy-=(1100-spaceship.y)/7500;
		if(this.x+this.velkostx/2<spaceship.x+spaceship.velkostx/2 && spaceship.vx>-2 && this.timer < 100)
			spaceship.vx-=(1100-spaceship.x)/7500;
		if(this.y+this.velkosty/2>spaceship.y+spaceship.velkosty/2 && spaceship.vy<2 && this.timer < 100)
			spaceship.vy+=(spaceship.y+100)/7500;
			
		if(this.timer<0)
		{
			this.timer=300;
		}
		if(this.timer_asteroid<0)
		{
			this.timer_asteroid=1000;
			asteroidy.push(asteroid());
			asteroidy[ktory-1].x=this.x+this.velkostx/2-asteroidy[ktory-1].velkostx/2;
			asteroidy[ktory-1].y=this.y+this.velkosty/2-asteroidy[ktory-1].velkosty/2;
		}
		if(this.timer_venom<0)
		{
			this.timer_venom=4000;
			asteroidy.push(venom());
			asteroidy[ktory-1].x=this.x+this.velkostx/2-asteroidy[ktory-1].velkostx/2;
			asteroidy[ktory-1].y=this.y+this.velkosty/2-asteroidy[ktory-1].velkosty/2;
		}
		if(this.timer_abom<0)
		{
			this.timer_abom=10000;
			asteroidy.push(abom());
			asteroidy[ktory-1].x=this.x+this.velkostx/2-asteroidy[ktory-1].velkostx/2;
			asteroidy[ktory-1].y=this.y+this.velkosty/2-asteroidy[ktory-1].velkosty/2;
		}
		if(this.timer_krystal<0)
		{
			this.timer_krystal=7500;
			asteroidy.push(krystal());
			asteroidy[ktory-1].x=this.x+this.velkostx/2-asteroidy[ktory-1].velkostx/2;
			asteroidy[ktory-1].y=this.y+this.velkosty/2-asteroidy[ktory-1].velkosty/2;
		}
		if(this.timer_veteran<0)
		{
			this.timer_veteran=5000;
			asteroidy.push(veteran());
			asteroidy[ktory-1].x=this.x+this.velkostx/2-asteroidy[ktory-1].velkostx/2;
			asteroidy[ktory-1].y=this.y+this.velkosty/2-asteroidy[ktory-1].velkosty/2;
		}
		//ma zivoty?
		if(this.velkostx<=0)
			delete(asteroidy[this.ktory]);
		//pozicia
		naboj.otocenie+=0.4;
		this.timer--;
		this.timer_asteroid--;
		this.timer_venom--;
		this.timer_abom--;
		this.timer_krystal--;
		this.timer_veteran--;
	}
	return naboj;
}

//generovanie venomov
function veteran(){
	var veteran={}
	veteran.x = -100+(ktory%2)*1000;
	veteran.y = Math.random()*950;
	veteran.vx = Math.random()*3-1;
	veteran.vy = Math.random()*3-1;
	veteran.velkostx=80;
	veteran.velkosty=120;
	veteran.texture = 88;
	veteran.otocenie = Math.random()*360;
	veteran.zivoty = 1800;
	veteran.maxzivoty = 1800;
	veteran.stit = 0;
	veteran.ktory = ktory++;
	veteran.vzdialenost =Math.random()*100+200;
	veteran.timer=Math.random()*3*200+100;
	veteran.timer2=0;//Math.random()*3*200+1000;
	
	//spravanie
	veteran.special= function(){
	},
	veteran.move=  function() {
	
		//ma zivoty?
		if(this.zivoty<=0)
		{
			spaceship.score+=3500;
			naboje.push(explo(veteran));
			delete(asteroidy[this.ktory]);
		}
		
		if(this.zivoty<this.maxzivoty/3)
		{
			this.texture=90;
			this.timer2-=3;
		}
		else if(this.zivoty<this.maxzivoty/3*2)
		{
			this.texture=89;
			this.timer--;
		}
			
		//pozicia
		this.x+=this.vx;
		this.y+=this.vy;
		
		var xt=(this.x+this.velkostx/2)-spaceship.xt;
		var yt=(this.y+this.velkosty/2)-spaceship.yt;
		var chceme=57.3*(Math.acos(yt/(Math.sqrt(xt*xt+yt*yt))));	//vypočítanie, aký uhol treba, aby mieril na hráča
		
		if(this.x>spaceship.x)
			chceme+=(180-chceme)*2;
		if(this.otocenie<chceme)
			this.otocenie+=0.25;
		else
			this.otocenie-=0.25;
		
		//logika hýbania sa
		if(this.x>spaceship.x)
		{
			if(this.x-this.vzdialenost>spaceship.x && this.vx>-0.25)
				this.vx-=0.02;
			else if(this.vx<0.5)
				this.vx+=0.02;		
		}
		else
		{
			if(this.x+this.vzdialenost<spaceship.x && this.vx<0.25)
				this.vx+=0.02;
			else if(this.vx>-0.5)
				this.vx-=0.02;	
		}
		if(this.y>spaceship.y)
		{
			if(this.y-this.vzdialenost>spaceship.y && this.vy>-0.25)
				this.vy-=0.02;
			else if(this.vy<0.5)
				this.vy+=0.02;		
		}
		else
		{
			if(this.y+this.vzdialenost<spaceship.y && this.vy<0.25)
				this.vy+=0.02;
			else if(this.vy>-0.5)
				this.vy-=0.02;	
		}
		
		
		//strielanie
		if(this.timer>0)
			this.timer--;
		if(this.timer<=0 && chceme-this.otocenie <5 && chceme-this.otocenie >-5)
		{
			asteroidy.push(enaboj(veteran));
			this.timer=400;
			this.ran=Math.random()*3+1;
		}
		
		if(this.timer2>0)
			this.timer2--;
		if(this.timer2<=0 && chceme-this.otocenie <5 && chceme-this.otocenie >-5)
		{
			asteroidy.push(eraketa(veteran));
			this.timer2=1000;
		}
		
		//logika okraja mapy
		if(this.x<-40)
			this.vx=1.5;
		if(this.y<-60)
			this.vy=1.5;
		if(this.x+this.velkostx>platno.width+40)
			this.vx=-1.5;
		if(this.y>platno.height-110)
			this.vy=-1.5;
			
		//logika otáčania
		if(this.otocenie>360)
			this.otocenie-=360;
		if(this.otocenie<-360)
			this.otocenie+=360;
	}
	return veteran;
}

//raketa 1
function eraketa(objekt){
	var naboj ={}
	naboj.x = objekt.x+objekt.velkostx/2-4;
	naboj.y = objekt.y+objekt.velkosty/2-15;
	naboj.vx = 0;
	naboj.vy = 0;
	naboj.velkostx=15;
	naboj.velkosty=60;
	naboj.texture = 87;
	naboj.otocenie = objekt.otocenie;
	naboj.zivoty = 750;
	naboj.maxzivoty = 750; //dmg naboju
	naboj.stit = 0;
	naboj.ktory = ktory++;
	naboj.special = function(){
	};
	//kam ma strielat
	if(naboj.otocenie>=0 && naboj.otocenie<=90)
		{
			naboj.vx=(naboj.otocenie*(1/90))*3;
			naboj.vy=(-90/90+naboj.otocenie*(1/90))*3;
		}
	else if(naboj.otocenie>90 && naboj.otocenie<=180)
		{
			naboj.vx=(90/90-(naboj.otocenie-90)*(1/90))*3;
			naboj.vy=((naboj.otocenie-90)*(1/90))*3;
		}
	else if(naboj.otocenie>180 && naboj.otocenie<=270)
		{
			naboj.vx=(-(naboj.otocenie-180)*(1/90))*3;
			naboj.vy=(90/90-(naboj.otocenie-180)*(1/90))*3;
		}
	else if(naboj.otocenie>270 && naboj.otocenie<=361)
		{
			naboj.vx=(-90/90+(naboj.otocenie-270)*(1/90))*3;
			naboj.vy=(-(naboj.otocenie-270)*(1/90))*3;
		}
	//spravanie
	naboj.move=  function() {
		//ma zivoty?
		if(this.zivoty<=0)
			delete(asteroidy[this.ktory]);
		//pozicia
		this.x+=this.vx;
		this.y+=this.vy;
		//logika okraja mapy
		if(this.x>platno.width+150)
			delete(asteroidy[this.ktory]);
		if(this.x<-150)
			delete(asteroidy[this.ktory]);
		if(this.y>platno.height+50)
			delete(asteroidy[this.ktory]);
		if(this.y<-150)
			delete(asteroidy[this.ktory]);
	}
	return naboj;
}

//generovanie výbušných škvŕn
function eexplor(objekt){
	var naboj ={}
	naboj.vx = Math.random()*0.5-0.25;
	naboj.vy = Math.random()*0.5-0.25;
	naboj.velkostx=5;
	naboj.velkosty=naboj.velkostx;
	naboj.x = objekt.x+objekt.velkostx/2-naboj.velkostx/2;
	naboj.y = objekt.y+objekt.velkosty/2-naboj.velkosty/2;
	naboj.texture = 82;
	naboj.otocenie = Math.random()*360;
	naboj.zivoty = 0;
	naboj.maxzivoty = objekt.maxzivoty/250; //dmg naboju
	naboj.stit = 0;
	naboj.ktory = ktory++;
	naboj.timer = 0;
	naboj.special = function(){
	};
	
	naboj.move=  function() {
		//ma zivoty?
		if(this.velkostx<=0)
			delete(asteroidy[this.ktory]);
		this.velkostx+=0.15;
		this.velkosty+=0.15;
		this.x-=0.075+this.vx;
		this.y-=0.075+this.vy;
		//pozicia
		naboj.otocenie+=0.15;
		this.timer++;
		if(this.timer>2000)
			delete(asteroidy[this.ktory]);
	}
	return naboj;
}

//generovanie nestabilných bytostí
function krystal(){
	var krystal={}
	krystal.x = -100+(ktory%2)*1000;
	krystal.y = Math.random()*950;
	krystal.vx = 0;
	krystal.vy = 0;
	krystal.velkostx=170;
	krystal.velkosty=145;
	krystal.texture = 81;
	krystal.otocenie = Math.random()*360;
	krystal.zivoty = 5000;
	krystal.maxzivoty = 2500;
	krystal.stit = 0;
	krystal.ktory = ktory++;
	krystal.timer=1300;
	if(zvukyon==1)
		krystal.k=0;
	krystal.zvuk=new Audio("sounds/crack_by_juanmerieventer.wav");
	
	//spravanie
	krystal.special= function(){
	},
	krystal.move=  function() {
	
		//ma zivoty?
		if(this.zivoty<=0)
		{
			spaceship.score+=7000;
			this.maxzivoty/=2;
			asteroidy.push(eexplor(krystal));
			delete(asteroidy[this.ktory]);
		}
		
		if(this.zivoty/2<this.maxzivoty/4)
		{
			this.texture=86;
			this.timer-=3;
			if(this.k==2)
			{
				krystal.zvuk.play();
				this.k++;
			}
		}
		else if(this.zivoty/2<this.maxzivoty/4*2){
			this.texture=85;this.timer-=1.3;
			if(this.k==1)
			{
				this.k++;
				krystal.zvuk.play();
			}
		}
		else if(this.zivoty/2<this.maxzivoty/4*3){
			this.texture=84;this.timer--;
			if(this.k==0)
			{
				this.k++;
				krystal.zvuk.play();
			}
		}
		
		//logika hýbania sa
		if(this.x+this.velkostx>spaceship.x+spaceship.velkostx && this.vx>-0.3)
			this.vx-=0.001;		
		else if(this.vx<0.3)
			this.vx+=0.001;
		if(this.y+this.velkosty>spaceship.y+spaceship.velkosty && this.vy>-0.3)
			this.vy-=0.001;
		else if(this.vy<0.3)
			this.vy+=0.001;
		
		//pozicia
		this.x+=this.vx;
		this.y+=this.vy;
		
		//strielanie
		if(this.timer>0)
			this.timer--;
		if(this.timer<=0)
		{
			asteroidy.push(eexplor(krystal));
			this.timer=1250;
		}
		
		//logika otáčania
		this.otocenie+=(Math.floor(Math.random()*2)-0.5)/2;
		if(this.otocenie>360)
			this.otocenie-=360;
		if(this.otocenie<-360)
			this.otocenie+=360;
	}
	return krystal;
}

//generovanie plazma výbuchov
function eexplop(objekt){
	var naboj ={}
	naboj.vx = 0;
	naboj.vy = 0;
	naboj.velkostx=objekt.maxzivoty/2+12;
	naboj.velkosty=naboj.velkostx;
	naboj.x = objekt.x+objekt.velkostx/2-naboj.velkostx/2;
	naboj.y = objekt.y+objekt.velkosty/2-naboj.velkosty/2;
	naboj.texture = 34;
	naboj.otocenie = Math.random()*360;
	naboj.zivoty = 0;
	naboj.maxzivoty = objekt.maxzivoty/50; //dmg naboju
	naboj.stit = 0;
	naboj.ktory = ktory++;
	naboj.special = function(){
	};
	
	naboj.move=  function() {
		//ma zivoty?
		if(this.velkostx<=0)
			delete(asteroidy[this.ktory]);
		this.velkostx-=0.15;
		this.velkosty-=0.15;
		this.x+=0.075;
		this.y+=0.075;
		//pozicia
		naboj.otocenie+=0.15;
	}
	return naboj;
}

//generovanie malých vesmírnych odporností
function small_abom(objekt){
	var small_abom={}
	small_abom.vx = 0;
	small_abom.vy = 0;
	small_abom.velkostx=45;
	small_abom.velkosty=45;
	small_abom.x = objekt.x+objekt.velkostx/2-small_abom.velkostx;
	small_abom.y = objekt.y+objekt.velkosty/2-small_abom.velkosty;
	small_abom.texture = 79;
	small_abom.otocenie = Math.random()*360;
	small_abom.zivoty = 450;
	small_abom.maxzivoty = 200;
	small_abom.stit = 0;
	small_abom.ktory = ktory++;
	
	//spravanie
	small_abom.special= function(){
	},
	small_abom.move=  function() {
	
		this.zivoty-=0.6;
		//ma zivoty?
		if(this.zivoty<=0)
		{
			spaceship.score+=0;
			this.maxzivoty/=2;
			asteroidy.push(eexplop(small_abom));
			delete(asteroidy[this.ktory]);
		}
		
		//logika hýbania sa
		if(this.x+this.velkostx>spaceship.x+spaceship.velkostx && this.vx>-1.5)
			this.vx-=0.05;		
		else if(this.vx<1.5)
			this.vx+=0.05;
		if(this.y+this.velkosty>spaceship.y+spaceship.velkosty && this.vy>-1.5)
			this.vy-=0.05;
		else if(this.vy<1.5)
			this.vy+=0.05;
		
		//pozicia
		this.x+=this.vx;
		this.y+=this.vy;
		
		//logika otáčania
		this.otocenie+=(Math.floor(Math.random()*21)-10)/2;
		if(this.otocenie>360)
			this.otocenie-=360;
		if(this.otocenie<-360)
			this.otocenie+=360;
	}
	return small_abom;
}
//generovanie vesmirnych odporností
function abom(){
	var abom={}
	abom.x = -100+(ktory%2)*1000;
	abom.y = Math.random()*950;
	abom.vx = Math.random()-0.5;
	abom.vy = Math.random()-0.5;
	abom.velkostx=200;
	abom.velkosty=200;
	abom.texture = 70;
	abom.otocenie = Math.random()*360;
	abom.zivoty = 3000;
	abom.maxzivoty = 3000;
	abom.stit = 0;
	abom.ktory = ktory++;
	abom.vzdialenost =200;
	abom.timer=100;
	
	//spravanie
	abom.special= function(){
	},
	abom.move=  function() {
	
		//ma zivoty?
		if(this.zivoty<=0)
		{
			spaceship.score+=6000;
			this.x+=50+Math.random()*50;
			this.y+=50+Math.random()*50;
			asteroidy.push(small_abom(abom));
			this.x-=100+Math.random()*50;
			asteroidy.push(small_abom(abom));
			this.y-=100+Math.random()*50;
			asteroidy.push(small_abom(abom));
			this.x+=100+Math.random()*50;
			asteroidy.push(small_abom(abom));
			this.x-=50+Math.random()*50;
			this.y+=50+Math.random()*50;
			this.maxzivoty/=10;
			asteroidy.push(eexplop(abom));
			delete(asteroidy[this.ktory]);
		}
		
		//logika hýbania sa
		if(this.x+this.velkostx/2>spaceship.x+spaceship.velkostx/2)
		{
			if(this.x+this.velkostx/2-this.vzdialenost<spaceship.x+spaceship.velkostx/2 && this.vx<0.5)
				this.vx+=0.01;		
		}
		else
		{
			if(this.x+this.velkostx/2+this.vzdialenost>spaceship.x+spaceship.velkostx/2 && this.vx>-0.5)
				this.vx-=0.01;	
		}
		if(this.y+this.velkosty/2>spaceship.y+spaceship.velkosty/2)
		{
			if(this.y+this.velkosty/2-this.vzdialenost<spaceship.y+spaceship.velkosty/2 && this.vy<0.5)
				this.vy+=0.01;		
		}
		else
		{
			if(this.y+this.velkosty/2+this.vzdialenost>spaceship.y+spaceship.velkosty/2 && this.vy>-0.5)
				this.vy-=0.01;	
		}
		
		
		//strielanie
		if(this.timer>0 && this.zivoty<this.maxzivoty)
			this.timer--;
		if(this.timer<=0 && this.zivoty<this.maxzivoty)
		{
			asteroidy.push(small_abom(abom));
			this.timer=700;
		}
		
		//textury
		if(this.zivoty<this.maxzivoty/9)
		{
			this.texture=78;
			this.vx=0;
			this.vy=0;
			this.timer-=2;
		}
		else if(this.zivoty<this.maxzivoty/9*2){
			this.texture=77;this.timer-=1.3}
		else if(this.zivoty<this.maxzivoty/9*3){
			this.texture=76;this.timer--;}
		else if(this.zivoty<this.maxzivoty/9*4){
			this.texture=75;this.timer-=0.8;}
		else if(this.zivoty<this.maxzivoty/9*5){
			this.texture=74;this.timer-=0.6;}
		else if(this.zivoty<this.maxzivoty/9*6){
			this.texture=73;this.timer-=0.4;}
		else if(this.zivoty<this.maxzivoty/9*7){
			this.texture=72;this.timer-=0.2;}
		else if(this.zivoty<this.maxzivoty/9*8)
			this.texture=71;
		//pozicia
		this.x+=this.vx;
		this.y+=this.vy;
		
		//logika okraja mapy
		if(this.x<-100)
			this.vx=0.5;
		if(this.y<-100)
			this.vy=0.5;
		if(this.x+this.velkostx>platno.width+100)
			this.vx=-0.5;
		if(this.y>platno.height-200)
			this.vy=-0.5;
		
		//logika otáčania
		this.otocenie+=(Math.floor(Math.random()*2)-0.5)/2;
		if(this.otocenie>360)
			this.otocenie-=360;
		if(this.otocenie<-360)
			this.otocenie+=360;
	}
	return abom;
}

//generovanie venomov
function venom(){
	var venom={}
	venom.x = -100+(ktory%2)*1000;
	venom.y = Math.random()*950;
	venom.vx = Math.random()*3-1;
	venom.vy = Math.random()*3-1;
	venom.velkostx=25;
	venom.velkosty=75;
	venom.texture = 54;
	venom.otocenie = Math.random()*360;
	venom.zivoty = 400;
	venom.maxzivoty = 1000;
	venom.stit = 200;
	venom.ktory = ktory++;
	venom.vzdialenost =Math.random()*100+350;
	venom.timer=Math.random()*3*200+100;
	venom.ran=Math.random()*7+1;
	
	//spravanie
	venom.special= function(){
	},
	venom.move=  function() {
	
		//ma zivoty?
		if(this.zivoty<=0)
		{
			spaceship.score+=1500;
			naboje.push(explo(venom));
			delete(asteroidy[this.ktory]);
		}
			
		//pozicia
		this.x+=this.vx;
		this.y+=this.vy;
		
		var xt=(this.x+this.velkostx/2)-spaceship.xt;
		var yt=(this.y+this.velkosty/2)-spaceship.yt;
		var chceme=57.3*(Math.acos(yt/(Math.sqrt(xt*xt+yt*yt))));	//vypočítanie, aký uhol treba, aby mieril na hráča
		
		if(this.x>spaceship.x)
			chceme+=(180-chceme)*2;
		if(this.otocenie<chceme)
			this.otocenie+=0.5;
		else
			this.otocenie-=0.5;
		
		//logika hýbania sa
		if(this.x>spaceship.x)
		{
			if(this.x-this.vzdialenost>spaceship.x && this.vx>-1)
				this.vx-=0.05;
			else if(this.vx<1)
				this.vx+=0.05;		
		}
		else
		{
			if(this.x+this.vzdialenost<spaceship.x && this.vx<1)
				this.vx+=0.05;
			else if(this.vx>-1)
				this.vx-=0.05;	
		}
		if(this.y>spaceship.y)
		{
			if(this.y-this.vzdialenost+this.ran*50>spaceship.y && this.vy>-1)
				this.vy-=0.05;
			else if(this.vy<1)
				this.vy+=0.05;		
		}
		else
		{
			if(this.y+this.vzdialenost-this.ran*50<spaceship.y && this.vy<1)
				this.vy+=0.05;
			else if(this.vy>-1)
				this.vy-=0.05;	
		}
		
		this.vzdialenost-=0.03;
		
		//strielanie
		if(this.timer>0)
			this.timer--;
		if(this.timer<=0 && chceme-this.otocenie <5 && chceme-this.otocenie >-5)
		{
			asteroidy.push(enaboj(venom));
			this.timer=500;
			this.ran=Math.random()*3+1;
		}
		
		//stitova textura
		if(this.stit>0)
			this.texture=56;
		else
			this.texture=54;
		
		//logika okraja mapy
		if(this.x<-40)
			this.vx=5.5;
		if(this.y<-60)
			this.vy=5.5;
		if(this.x+this.velkostx>platno.width+40)
			this.vx=-5.5;
		if(this.y>platno.height-110)
			this.vy=-5.5;
			
		//logika otáčania
		if(this.otocenie>360)
			this.otocenie-=360;
		if(this.otocenie<-360)
			this.otocenie+=360;
	}
	return venom;
}

//laser 1
function enaboj(objekt){
	var naboj ={}
	naboj.x = objekt.x+objekt.velkostx/2-4;
	naboj.y = objekt.y+objekt.velkosty/2-15;
	naboj.vx = 0;
	naboj.vy = 0;
	naboj.velkostx=8;
	naboj.velkosty=30;
	naboj.texture = 55;
	naboj.otocenie = objekt.otocenie;
	naboj.zivoty = 150;
	naboj.maxzivoty = 150; //dmg naboju
	naboj.stit = 0;
	naboj.ktory = ktory++;
	naboj.special = function(){
	};
	//kam ma strielat
	if(naboj.otocenie>=0 && naboj.otocenie<=90)
		{
			naboj.vx=(naboj.otocenie*(1/90))*6;
			naboj.vy=(-90/90+naboj.otocenie*(1/90))*6;
		}
	else if(naboj.otocenie>90 && naboj.otocenie<=180)
		{
			naboj.vx=(90/90-(naboj.otocenie-90)*(1/90))*6;
			naboj.vy=((naboj.otocenie-90)*(1/90))*6;
		}
	else if(naboj.otocenie>180 && naboj.otocenie<=270)
		{
			naboj.vx=(-(naboj.otocenie-180)*(1/90))*6;
			naboj.vy=(90/90-(naboj.otocenie-180)*(1/90))*6;
		}
	else if(naboj.otocenie>270 && naboj.otocenie<=361)
		{
			naboj.vx=(-90/90+(naboj.otocenie-270)*(1/90))*6;
			naboj.vy=(-(naboj.otocenie-270)*(1/90))*6;
		}
	//spravanie
	naboj.move=  function() {
		//ma zivoty?
		if(this.zivoty<=0)
			delete(asteroidy[this.ktory]);
		//pozicia
		this.x+=this.vx;
		this.y+=this.vy;
		//logika okraja mapy
		if(this.x>platno.width+150)
			delete(asteroidy[this.ktory]);
		if(this.x<-150)
			delete(asteroidy[this.ktory]);
		if(this.y>platno.height+50)
			delete(asteroidy[this.ktory]);
		if(this.y<-150)
			delete(asteroidy[this.ktory]);
	}
	return naboj;
}

//generovanie asteroidov
function asteroid(){
	var asteroid ={}
	asteroid.x = -150+(ktory%2)*1150;
	asteroid.y = Math.random()*950;
	asteroid.vx = Math.random()*3.5-1.5;
	asteroid.vy = Math.random()*3.5-1.5;
	if((asteroid.vx<0.1 && asteroid.vx>-0.1) || (asteroid.vy<0.1 && asteroid.vy>-0.1))
	{
		asteroid.vx=0.2;
		asteroid.vy=0.2;
	}
	asteroid.velkostx=180-(Math.abs(asteroid.vy)+Math.abs(asteroid.vx))*40;
	asteroid.velkosty=asteroid.velkostx;
	asteroid.texture = Math.floor(Math.random()*3);
	asteroid.tex = asteroid.texture;
	asteroid.otocenie = Math.random()*360;
	asteroid.zivoty = asteroid.velkostx*4;
	asteroid.maxzivoty = asteroid.zivoty*1.2;
	asteroid.stit = 0;
	asteroid.ktory = ktory++;
	
	//spravanie
	asteroid.special= function(){
	},
	asteroid.move=  function() {
	
		//ma zivoty?
		if(this.zivoty<=0)
		{
			spaceship.score+=Math.floor(this.velkostx*3/10)*15;
			naboje.push(explo(asteroid));
			delete(asteroidy[this.ktory]);
		}
		if(this.zivoty<=this.maxzivoty/3*2)
			this.texture=this.tex+60;
		if(this.zivoty<=this.maxzivoty/3)
			this.texture=this.tex+63;
			
		//pozicia
		this.x+=this.vx;
		this.y+=this.vy;
		if(this.vx>0)
			this.otocenie+=(175-this.velkostx)/100;
		if(this.vx<0)
			this.otocenie-=(175-this.velkostx)/100;
			
		//logika okraja mapy
		if(this.x>platno.width)
			this.x=-160;
		if(this.x<-160)
			this.x=platno.width;
		if(this.y>platno.height-100)
			this.y=-160;
		if(this.y<-160)
			this.y=platno.height-100;
			
		//logika otáčania
		if(this.otocenie>360)
			this.otocenie-=360;
		if(this.otocenie<-360)
			this.otocenie+=360;
	}
	return asteroid;
}