//pole na funkcie jednotlivých nábojov
var typy=[];
typy[0]=naboj;
typy[1]=raketa;
typy[2]=dvojity;
typy[3]=plamenomet;
typy[4]=gamma;
typy[5]=bioskin;
typy[6]=gulomet;
typy[7]=railgun;
typy[8]=epsilon;
typy[9]=nuke;

function explor(objekt){
	var naboj ={}
	naboj.vx = Math.random()*0.5-0.25;
	naboj.vy = Math.random()*0.5-0.25;
	naboj.velkostx=5;
	naboj.velkosty=naboj.velkostx;
	naboj.x = objekt.x+objekt.velkostx/2-naboj.velkostx/2;
	naboj.y = objekt.y+objekt.velkosty/2-naboj.velkosty/2;
	naboj.texture = 36;
	naboj.otocenie = Math.random()*360;
	naboj.zivoty = 0;
	naboj.maxzivoty = objekt.maxzivoty/200; //dmg naboju
	naboj.stit = 0;
	naboj.ktora = ktora++;
	naboj.timer = 0;
	naboj.special = function(){
	};
	
	naboj.move=  function() {
		//ma zivoty?
		if(this.velkostx<=0)
			delete(naboje[this.ktora]);
		this.velkostx+=0.15;
		this.velkosty+=0.15;
		this.x-=0.075+this.vx;
		this.y-=0.075+this.vy;
		//pozicia
		naboj.otocenie+=0.15;
		this.timer++;
		if(this.timer>900+objekt.skill*200)
			delete(naboje[this.ktora]);
	}
	return naboj;
}

function explop(objekt){
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
	naboj.ktora = ktora++;
	naboj.special = function(){
	};
	
	naboj.move=  function() {
		//ma zivoty?
		if(this.velkostx<=0)
			delete(naboje[this.ktora]);
		this.velkostx-=0.15;
		this.velkosty-=0.15;
		this.x+=0.075;
		this.y+=0.075;
		//pozicia
		naboj.otocenie+=0.15;
	}
	return naboj;
}

function explo(objekt){
	var naboj ={}
	naboj.vx = 0;
	naboj.vy = 0;
	naboj.velkostx=(Math.random()*3+5)*7;
	naboj.x = objekt.x+objekt.velkostx/2-naboj.velkostx/2;
	naboj.y = objekt.y+objekt.velkosty/2-naboj.velkostx/2;
	naboj.velkosty=naboj.velkostx;
	naboj.texture = Math.floor(Math.random()*3)+18;
	naboj.otocenie = spaceship.otocenie;
	naboj.zivoty = 0;
	naboj.maxzivoty = 0; //dmg naboju
	naboj.stit = 0;
	naboj.ktora = ktora++;
	naboj.special = function(){
	};
	if(zvukyon==1)
	{
		var zvuk=new Audio("sounds/explo_by_bareform.mp3");
		zvuk.play();
	}
	naboj.move=  function() {
		//ma zivoty?
		if(this.velkostx<=0)
			delete(naboje[this.ktora]);
		this.velkostx-=0.5;
		this.velkosty-=0.5;
		this.x+=0.25;
		this.y+=0.25;
		//pozicia
		naboj.otocenie+=0.15;
	}
	return naboj;
}

//generovanie sekundarnych

function nuke(){
//kontrola, či je dosť energie
	var skill=vylepsenie[13];
	if(spaceship.stit<=349+skill*50)
		return -1;
	spaceship.stit-=350+skill*50;
	
	//zbran cd
	spaceship.zbran2_cd=5000-300*skill;
	spaceship.cd2=spaceship.zbran2_cd;
	var naboj ={}
	naboj.x = spaceship.x+spaceship.velkostx/2-10;
	naboj.y = spaceship.y+spaceship.velkosty/2-35;
	naboj.vx = 0;
	naboj.vy = 0;
	naboj.velkostx=20;
	naboj.velkosty=70;
	naboj.texture = 35;
	naboj.otocenie = spaceship.otocenie;
	naboj.zivoty = 451;
	naboj.maxzivoty = 400+200*skill; //dmg naboju
	naboj.stit = 0;
	naboj.ktora = ktora++;
	naboj.skill=skill;
	naboj.special = function(){
	};
	//kam ma strielat
	if(naboj.otocenie>=0 && naboj.otocenie<=90)
		{
			naboj.vx=(naboj.otocenie*(1/90))*1.5;
			naboj.vy=(-90/90+naboj.otocenie*(1/90))*1.5;
		}
	else if(naboj.otocenie>90 && naboj.otocenie<=180)
		{
			naboj.vx=(90/90-(naboj.otocenie-90)*(1/90))*1.5;
			naboj.vy=((naboj.otocenie-90)*(1/90))*1.5;
		}
	else if(naboj.otocenie>180 && naboj.otocenie<=270)
		{
			naboj.vx=(-(naboj.otocenie-180)*(1/90))*1.5;
			naboj.vy=(90/90-(naboj.otocenie-180)*(1/90))*1.5;
		}
	else if(naboj.otocenie>270 && naboj.otocenie<=361)
		{
			naboj.vx=(-90/90+(naboj.otocenie-270)*(1/90))*1.5;
			naboj.vy=(-(naboj.otocenie-270)*(1/90))*1.5;
		}
	//spravanie
	naboj.move=  function() {
		//ma zivoty?
		if(this.zivoty<=0)
		{
			naboje.push(explo(naboj));
			this.maxzivoty/=2;
			naboje.push(explor(naboj));
			naboje.push(explo(naboj));
			delete(naboje[this.ktora]);
		}
		//pozicia
		this.x+=this.vx;
		this.y+=this.vy;
		//logika okraja mapy
		if(this.x>platno.width+80)
			delete(naboje[this.ktora]);
		if(this.x<-80)
			delete(naboje[this.ktora]);
		if(this.y>platno.height)
			delete(naboje[this.ktora]);
		if(this.y<-80)
			delete(naboje[this.ktora]);
	}
	return naboj;
}

function railgun(){
//kontrola, či je dosť energie
	var skill=vylepsenie[12];
	if(spaceship.stit<=50+skill*50)
		return -1;
	spaceship.stit-=50+skill*50;
	
	if(zvukyon==1)
	{
		var zvuk=new Audio("sounds/railgun_by_robinhood76.wav");
		zvuk.volume=0.5;
		zvuk.play();
	}
	
	//zbran cd
	spaceship.zbran2_cd=1200-100*skill;
	spaceship.cd2=spaceship.zbran2_cd;
	var naboj ={}
	naboj.x = spaceship.x+spaceship.velkostx/2-10;
	naboj.y = spaceship.y+spaceship.velkosty/2-40;
	naboj.vx = 0;
	naboj.vy = 0;
	naboj.velkostx=20;
	naboj.velkosty=80;
	naboj.texture = 32;
	naboj.otocenie = spaceship.otocenie;
	naboj.zivoty = 0;
	naboj.maxzivoty = 5+spaceship.stit*(0.4+skill*0.10)/20; //dmg naboju
	naboj.stit = 0;
	naboj.ktora = ktora++;
	naboj.timer = 0;
	naboj.i = 1;
	naboj.special = function(){
	};
	//kam ma strielat
	if(naboj.otocenie>=0 && naboj.otocenie<=90)
		{
			naboj.vx=(naboj.otocenie*(1/90))*8;
			naboj.vy=(-90/90+naboj.otocenie*(1/90))*8;
		}
	else if(naboj.otocenie>90 && naboj.otocenie<=180)
		{
			naboj.vx=(90/90-(naboj.otocenie-90)*(1/90))*8;
			naboj.vy=((naboj.otocenie-90)*(1/90))*8;
		}
	else if(naboj.otocenie>180 && naboj.otocenie<=270)
		{
			naboj.vx=(-(naboj.otocenie-180)*(1/90))*8;
			naboj.vy=(90/90-(naboj.otocenie-180)*(1/90))*8;
		}
	else if(naboj.otocenie>270 && naboj.otocenie<=361)
		{
			naboj.vx=(-90/90+(naboj.otocenie-270)*(1/90))*8;
			naboj.vy=(-(naboj.otocenie-270)*(1/90))*8;
		}
	
	//spravanie
	naboj.move=  function() {
		//textura
		this.timer+=this.i;
		if(this.timer==12)
		{
			this.i*=-1;
			this.texture=33;
			naboje.push(explop(naboj));
		}
		if(this.timer==0)
		{
			this.i*=-1;
			this.texture=32;
			naboje.push(explop(naboj));
		}

		//pozicia
		this.x+=this.vx;
		this.y+=this.vy;
		//logika okraja mapy
		if(this.x>platno.width+150)
			delete(naboje[this.ktora]);
		if(this.x<-150)
			delete(naboje[this.ktora]);
		if(this.y>platno.height+50)
			delete(naboje[this.ktora]);
		if(this.y<-150)
			delete(naboje[this.ktora]);
	}
	spaceship.stit=0;
	return naboj;
}

function bioskin(){
	var skill=vylepsenie[11];
	//kontrola, či je dosť energie
	if(spaceship.stit<spaceship.maxstit-spaceship.maxstit/(10+skill) || spaceship.zivoty >= spaceship.maxzivoty)
		return -1;
	spaceship.stit-=100+spaceship.maxstit/(10+skill);
	
	if(zvukyon==1)
	{
		var zvuk=new Audio("sounds/bioskin_by_dpoggioli.mp3");
		zvuk.play();
	}
	
	//zbran cd
	spaceship.zbran2_cd=2000-100*skill;
	var naboj ={}
	naboj.x = 1500;
	naboj.y = 1500;
	naboj.velkostx=1;
	naboj.velkosty=1;
	naboj.texture = 17;
	naboj.zivoty = 0.1;
	naboj.maxzivoty = 0*skill; //dmg naboju
	naboj.stit = 0;
	naboj.ktora = ktora++;
	naboj.special = function(){
	};
	
	//spravanie
	naboj.move=  function() {
		spaceship.cd2=spaceship.zbran2_cd;
		//ma zivoty?
		if(spaceship.stit<=0 || keys[67]==false || spaceship.zivoty>=spaceship.maxzivoty)
			delete(naboje[this.ktora]);
			
		stit.texture=31;
		
		//drain a repair
		spaceship.stit-=1+skill*0.3;
		if(spaceship.zivoty<spaceship.maxzivoty)
			spaceship.zivoty+=0.5+skill*0.3;
	}
	return naboj;
}

function plamenomet(){
	var skill=vylepsenie[10];
	//kontrola, či je dosť energie
	if(spaceship.stit<=50+skill*50)
		return -1;
	spaceship.stit-=50+skill*20;
	
	//zbran cd
	spaceship.zbran2_cd=700-50*skill;
	//spaceship.cd2=spaceship.zbran2_cd; //bude v move
	var naboj ={}
	naboj.x = spaceship.x+spaceship.velkostx/2-this.velkostx/2;
	naboj.y = spaceship.y-this.velkosty;
	naboj.vx = 0;
	naboj.vy = 0;
	naboj.velkostx=0;
	naboj.velkosty=200;
	naboj.texture = 29;
	naboj.otocenie = spaceship.otocenie;
	naboj.zivoty = 0;
	naboj.maxzivoty = 1.5+0.5*skill; //dmg naboju
	naboj.stit = 0;
	naboj.ktora = ktora++;
	naboj.timer=0;
	naboj.i=1;
	naboj.special = function(){
	};
	//kam ma strielat
	
	//spravanie
	naboj.move=  function() {
		spaceship.cd2=spaceship.zbran2_cd;
		naboj.otocenie=spaceship.otocenie
		if(this.velkostx<200)
			this.velkostx+=4;
		//ma zivoty?
		if(spaceship.stit<=0 || keys[67]==false)
			delete(naboje[this.ktora]);
		//pozicia
		this.x=spaceship.x+spaceship.velkostx/2-this.velkostx/2;
		this.y=spaceship.y+spaceship.velkosty/2-this.velkosty/2;
		//uprava pozicie
		var rozdiel;
		if(this.otocenie>=0 && this.otocenie<=90)
			{
				if(this.otocenie<=45)
					rozdiel=this.otocenie;
				else
					rozdiel=90-this.otocenie;
					
				this.x+=this.otocenie*(1.4+rozdiel*0.015);
				this.y+=(this.otocenie-90)*(1.4+rozdiel*0.015);
			}
		else if(this.otocenie>90 && this.otocenie<=180)
			{
				if(this.otocenie-90<=45)
					rozdiel=this.otocenie-90;
				else
					rozdiel=90-(this.otocenie-90);
					
				this.x+=(this.otocenie-(this.otocenie-90)*2)*(1.4+rozdiel*0.015);
				this.y+=(this.otocenie-90)*(1.4+rozdiel*0.015);
			}
		else if(this.otocenie>180 && this.otocenie<=270)
			{
				if(this.otocenie-180<=45)
					rozdiel=this.otocenie-180;
				else
					rozdiel=90-(this.otocenie-180);
					
				this.x-=(this.otocenie-180)*(1.4+rozdiel*0.015);
				this.y+=(this.otocenie-90-(this.otocenie-180)*2)*(1.4+rozdiel*0.015);
			}
		else if(this.otocenie>270 && this.otocenie<=361)
			{
				if(this.otocenie-270<=45)
					rozdiel=this.otocenie-270;
				else
					rozdiel=90-(this.otocenie-270);
				this.x+=(this.otocenie-360)*(1.4+rozdiel*0.015);
				this.y+=(this.otocenie-90-(this.otocenie-180)*2)*(1.4+rozdiel*0.015);
			}
		//shield drain
		spaceship.stit-=2.5-skill*0.25;
		//textúra ohňa
		this.timer+=this.i;
		
		if(this.timer==10)
		{
			this.texture=30;
			this.i*=-1;
		}
		else if(this.timer==0)
		{
			this.texture=29;
			this.i*=-1;
		}
	}
	naboj.move();
	return naboj;
}

function raketa(){
	var skill=vylepsenie[9];
//kontrola, či je dosť energie
	if(spaceship.stit<=70+skill*50)
		return -1;
	spaceship.stit-=70+skill*50;
	
	if(zvukyon==1)
	{
		var zvuk=new Audio("sounds/launch_by_jorgerosa.mp3");
		zvuk.play();
	}
	
	//zbran cd
	spaceship.zbran2_cd=180-15*skill;
	spaceship.cd2=spaceship.zbran2_cd;
	var naboj ={}
	naboj.x = spaceship.x+spaceship.velkostx/2-5;
	naboj.y = spaceship.y+spaceship.velkosty/2-19;
	naboj.vx = 0;
	naboj.vy = 0;
	naboj.velkostx=10;
	naboj.velkosty=37;
	naboj.texture = 17;
	naboj.otocenie = spaceship.otocenie;
	naboj.zivoty = 0.1;
	naboj.maxzivoty = 100+50*skill; //dmg naboju
	naboj.stit = 0;
	naboj.ktora = ktora++;
	naboj.special = function(){
	};
	//kam ma strielat
	if(naboj.otocenie>=0 && naboj.otocenie<=90)
		{
			naboj.vx=(naboj.otocenie*(1/90))*5;
			naboj.vy=(-90/90+naboj.otocenie*(1/90))*5;
		}
	else if(naboj.otocenie>90 && naboj.otocenie<=180)
		{
			naboj.vx=(90/90-(naboj.otocenie-90)*(1/90))*5;
			naboj.vy=((naboj.otocenie-90)*(1/90))*5;
		}
	else if(naboj.otocenie>180 && naboj.otocenie<=270)
		{
			naboj.vx=(-(naboj.otocenie-180)*(1/90))*5;
			naboj.vy=(90/90-(naboj.otocenie-180)*(1/90))*5;
		}
	else if(naboj.otocenie>270 && naboj.otocenie<=361)
		{
			naboj.vx=(-90/90+(naboj.otocenie-270)*(1/90))*5;
			naboj.vy=(-(naboj.otocenie-270)*(1/90))*5;
		}
	//spravanie
	naboj.move=  function() {
		//ma zivoty?
		if(this.zivoty<=0)
		{
			naboje.push(explo(naboj));
			delete(naboje[this.ktora]);
		}
		//pozicia
		this.x+=this.vx;
		this.y+=this.vy;
		//logika okraja mapy
		if(this.x>platno.width+150)
			this.zivoty=0;
		if(this.x<-150)
			this.zivoty=0;
		if(this.y>platno.height+50)
			this.zivoty=0;
		if(this.y<-150)
			this.zivoty=0;
	}
	return naboj;
}

//generovanie nabojov

//Epsilon
function epsilon(){
	var skill=vylepsenie[6];
	var naboj ={}
	naboj.x = spaceship.x+spaceship.velkostx/2-10;
	naboj.y = spaceship.y+spaceship.velkosty/2-15;
	naboj.vx = 0;
	naboj.vy = 0;
	naboj.velkostx=20;
	naboj.velkosty=30;
	naboj.texture = 28;
	naboj.otocenie = spaceship.otocenie;
	naboj.zivoty = 0;
	naboj.maxzivoty = 2.5+skill*0.5; //dmg naboju
	naboj.stit = 0;
	naboj.ktora = ktora++;
	naboj.special = function(){
		if(spaceship.stit<spaceship.maxstit)
			spaceship.stit+=1.5+skill*0.5;
	};
	//zbran cd
	spaceship.zbran1_cd=120-skill*7;	
	spaceship.cd1=spaceship.zbran1_cd;
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
		//pozicia
		//this.velkostx
		this.velkostx+=2/6;
		this.velkosty+=3/4;
		this.x+=this.vx;
		this.y+=this.vy;
		//logika okraja mapy
		if(this.x>platno.width+150)
			delete(naboje[this.ktora]);
		if(this.x<-150)
			delete(naboje[this.ktora]);
		if(this.y>platno.height+50)
			delete(naboje[this.ktora]);
		if(this.y<-150)
			delete(naboje[this.ktora]);
	}
	return naboj;
}

//Gulomet
function gulomet(){
	var skill=vylepsenie[5];
	var naboj ={}
	naboj.x = spaceship.x+spaceship.velkostx/2-3;
	naboj.y = spaceship.y+spaceship.velkosty/2-10;
	naboj.vx = 0;
	naboj.vy = 0;
	naboj.velkostx=6;
	naboj.velkosty=20;
	naboj.texture = 27;
	naboj.otocenie = spaceship.otocenie;
	naboj.zivoty = 0.1;
	naboj.maxzivoty = 15+skill*2; //dmg naboju
	naboj.stit = 0;
	naboj.ktora = ktora++;
	naboj.special = function(){
	};
	//zbran cd
	spaceship.zbran1_cd=15-Math.floor(skill*1.75);
	spaceship.cd1=spaceship.zbran1_cd;
	//kam ma strielat
	naboj.otocenie+=Math.random()*41-20;
	if(naboj.otocenie>360)
		naboj.otocenie-=360;
	if(naboj.otocenie<0)
		naboj.otocenie+=360;
	if(naboj.otocenie>=0 && naboj.otocenie<=90)
		{
			naboj.vx=(naboj.otocenie*(1/90))*10;
			naboj.vy=(-90/90+naboj.otocenie*(1/90))*10;
		}
	else if(naboj.otocenie>90 && naboj.otocenie<=180)
		{
			naboj.vx=(90/90-(naboj.otocenie-90)*(1/90))*10;
			naboj.vy=((naboj.otocenie-90)*(1/90))*10;
		}
	else if(naboj.otocenie>180 && naboj.otocenie<=270)
		{
			naboj.vx=(-(naboj.otocenie-180)*(1/90))*10;
			naboj.vy=(90/90-(naboj.otocenie-180)*(1/90))*10;
		}
	else if(naboj.otocenie>270 && naboj.otocenie<=361)
		{
			naboj.vx=(-90/90+(naboj.otocenie-270)*(1/90))*10;
			naboj.vy=(-(naboj.otocenie-270)*(1/90))*10;
		}
	//spravanie
	naboj.move=  function() {
		//ma zivoty?
		if(this.zivoty<=0)
			delete(naboje[this.ktora]);
		//pozicia
		this.x+=this.vx;
		this.y+=this.vy;
		//logika okraja mapy
		if(this.x>platno.width+150)
			this.zivoty=0;
		if(this.x<-150)
			this.zivoty=0;
		if(this.y>platno.height+50)
			this.zivoty=0;
		if(this.y<-150)
			this.zivoty=0;
	}
	return naboj;
}

//Gamma
function gamma(){
	var skill=vylepsenie[4];
	spaceship.cd1=spaceship.zbran1_cd;
	var naboj ={}
	naboj.x = spaceship.x+spaceship.velkostx/2-10;
	naboj.y = spaceship.y+spaceship.velkosty/2-15;
	naboj.vx = 0;
	naboj.vy = 0;
	naboj.velkostx=20;
	naboj.velkosty=30;
	naboj.texture = 26;
	naboj.otocenie = spaceship.otocenie;
	naboj.zivoty = 0;
	naboj.maxzivoty = 3+skill*0.75; //dmg naboju
	naboj.stit = 0;
	naboj.ktora = ktora++;
	naboj.special = function(){
	};
	//zbran cd
	spaceship.zbran1_cd=100-skill*7;	
	spaceship.cd1=spaceship.zbran1_cd;
	//kam ma strielat
	if(naboj.otocenie>=0 && naboj.otocenie<=90)
		{
			naboj.vx=(naboj.otocenie*(1/90))*7;
			naboj.vy=(-90/90+naboj.otocenie*(1/90))*7;
		}
	else if(naboj.otocenie>90 && naboj.otocenie<=180)
		{
			naboj.vx=(90/90-(naboj.otocenie-90)*(1/90))*7;
			naboj.vy=((naboj.otocenie-90)*(1/90))*7;
		}
	else if(naboj.otocenie>180 && naboj.otocenie<=270)
		{
			naboj.vx=(-(naboj.otocenie-180)*(1/90))*7;
			naboj.vy=(90/90-(naboj.otocenie-180)*(1/90))*7;
		}
	else if(naboj.otocenie>270 && naboj.otocenie<=361)
		{
			naboj.vx=(-90/90+(naboj.otocenie-270)*(1/90))*7;
			naboj.vy=(-(naboj.otocenie-270)*(1/90))*7;
		}
	//spravanie
	naboj.move=  function() {
		//pozicia
		//this.velkostx
		this.velkostx+=2/6;
		this.velkosty+=3/4;
		this.x+=this.vx;
		this.y+=this.vy;
		//logika okraja mapy
		if(this.x>platno.width+150)
			delete(naboje[this.ktora]);
		if(this.x<-150)
			delete(naboje[this.ktora]);
		if(this.y>platno.height+50)
			delete(naboje[this.ktora]);
		if(this.y<-150)
			delete(naboje[this.ktora]);
	}
	return naboj;
}

//trojity :)
function dvojity(k){
	var skill=vylepsenie[3];
	spaceship.cd1=spaceship.zbran1_cd;
	var naboj ={}
	naboj.x = spaceship.x+spaceship.velkostx/2-4;
	naboj.y = spaceship.y+spaceship.velkosty/2-15;
	naboj.vx = 0;
	naboj.vy = 0;
	naboj.velkostx=8;
	naboj.velkosty=30;
	naboj.texture = 25;
	naboj.otocenie = spaceship.otocenie;
	naboj.zivoty = 0.1;
	naboj.maxzivoty = 50+skill*15; //dmg naboju
	naboj.stit = 0;
	naboj.ktora = ktora++;
	naboj.special = function(){
	};
	
	if(zvukyon==1)
	{
		var zvuk=new Audio("sounds/laser_by_julien_matthey.mp3");
		zvuk.volume=0.3;
		zvuk.play();
	}
	
	//zbran cd
	spaceship.zbran1_cd=100+skill*35;
	spaceship.cd1=spaceship.zbran1_cd;
	//kam ma strielat
	if(naboj.otocenie>=0 && naboj.otocenie<=90)
		{
			naboj.vx=(naboj.otocenie*(1/90))*11;
			naboj.vy=(-90/90+naboj.otocenie*(1/90))*11;
		}
	else if(naboj.otocenie>90 && naboj.otocenie<=180)
		{
			naboj.vx=(90/90-(naboj.otocenie-90)*(1/90))*11;
			naboj.vy=((naboj.otocenie-90)*(1/90))*11;
		}
	else if(naboj.otocenie>180 && naboj.otocenie<=270)
		{
			naboj.vx=(-(naboj.otocenie-180)*(1/90))*11;
			naboj.vy=(90/90-(naboj.otocenie-180)*(1/90))*11;
		}
	else if(naboj.otocenie>270 && naboj.otocenie<=361)
		{
			naboj.vx=(-90/90+(naboj.otocenie-270)*(1/90))*11;
			naboj.vy=(-(naboj.otocenie-270)*(1/90))*11;
		}
	//spravanie
	naboj.move=  function() {
		//ma zivoty?
		this.zivoty-=0.0025;
		if(this.zivoty<=0)
			delete(naboje[this.ktora]);
		//pozicia
		this.x+=this.vx;
		this.y+=this.vy;
		//logika okraja mapy
		if(this.x>platno.width+150)
			this.zivoty=0;
		if(this.x<-150)
			this.zivoty=0;
		if(this.y>platno.height+50)
			this.zivoty=0;
		if(this.y<-150)
			this.zivoty=0;
	}
	if(k==undefined)
		k=0;
	else
		k++;
	if(k<1+skill*2)
		setTimeout(function(){naboje.push(dvojity(k))},120);
	return naboj;
}

//laser 1
function naboj(){
	var skill=vylepsenie[2];
	spaceship.cd1=spaceship.zbran1_cd;
	var naboj ={}
	naboj.x = spaceship.x+spaceship.velkostx/2-4;
	naboj.y = spaceship.y+spaceship.velkosty/2-15;
	naboj.vx = 0;
	naboj.vy = 0;
	naboj.velkostx=8;
	naboj.velkosty=30;
	naboj.texture = 12;
	naboj.otocenie = spaceship.otocenie;
	naboj.zivoty = 0.1;
	naboj.maxzivoty = 25+skill*10; //dmg naboju
	naboj.stit = 0;
	naboj.ktora = ktora++;
	naboj.special = function(){
	};
	
	if(zvukyon==1)
	{
		var zvuk=new Audio("sounds/laser_by_julien_matthey.mp3");
		zvuk.volume=0.3;
		zvuk.play();
	}
	
	//zbran cd
	spaceship.zbran1_cd=50-skill*4;	//godmode
	spaceship.cd1=spaceship.zbran1_cd;
	//kam ma strielat
	if(naboj.otocenie>=0 && naboj.otocenie<=90)
		{
			naboj.vx=(naboj.otocenie*(1/90))*8;
			naboj.vy=(-90/90+naboj.otocenie*(1/90))*8;
		}
	else if(naboj.otocenie>90 && naboj.otocenie<=180)
		{
			naboj.vx=(90/90-(naboj.otocenie-90)*(1/90))*8;
			naboj.vy=((naboj.otocenie-90)*(1/90))*8;
		}
	else if(naboj.otocenie>180 && naboj.otocenie<=270)
		{
			naboj.vx=(-(naboj.otocenie-180)*(1/90))*8;
			naboj.vy=(90/90-(naboj.otocenie-180)*(1/90))*8;
		}
	else if(naboj.otocenie>270 && naboj.otocenie<=361)
		{
			naboj.vx=(-90/90+(naboj.otocenie-270)*(1/90))*8;
			naboj.vy=(-(naboj.otocenie-270)*(1/90))*8;
		}
	//spravanie
	naboj.move=  function() {
		//ma zivoty?
		if(this.zivoty<=0)
			delete(naboje[this.ktora]);
		//pozicia
		this.x+=this.vx;
		this.y+=this.vy;
		//logika okraja mapy
		if(this.x>platno.width+150)
			this.zivoty=0;
		if(this.x<-150)
			this.zivoty=0;
		if(this.y>platno.height+50)
			this.zivoty=0;
		if(this.y<-150)
			this.zivoty=0;
	}
	return naboj;
}