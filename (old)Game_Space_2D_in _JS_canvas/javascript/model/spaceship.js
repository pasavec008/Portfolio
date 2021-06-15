//pozadie
var pozadie ={
	texture: 15,
};

//stit
var stit ={
	x: 500,
	y: 350,
	otocenie:0,
	velkostx: 50,
	velkosty: 91,
	texture: 13,
};

//spaceship
var spaceship ={
	x: 500,
	y: 350,
	vx:0,
	vy:0,
	texture: 3,
	texture_timer:0,
	otocenie: 0,
	velkostx: 30,
	velkosty: 71,
	turbo: 0.6,
	pohon: 0.5,
	maxzivoty: 1000,
	maxstit: 500,
	zivoty: 1000,
	zivotypred: 500,
	stit: 500,
	stitpred: 500,
	timer: 0,
	dobit: 0,
	naraz: 100,
	zbran1: 0,
	zbran2: 1,
	zbran1_cd:0,
	zbran2_cd:0,
	cd1: 0,
	cd2: 0,
	score: 0,
	xt:0,
	yt:0,
	uroven:0,
	akturoven:1,
	zvuk:0,
	track:new Audio("sounds/engine_by_anoesj.mp3"),
	
	special: function(){
		this.timer=10;
		stit.texture=14;
	},
	
	
	move: function(){
		this.x+=this.vx;
		this.y+=this.vy;
		this.xt=this.x+this.velkostx/2;
		this.yt=this.y+this.velkosty/2;
		//kontrola zivotov
		if(this.zivoty<=0)
		{
			this.track.pause();
			pauza=-5;
			setTimeout(function (){pauza=-2}, 2000);
		}
		//funkcia pasívneho dobíjania štítu
		if(this.stit<this.stitpred || this.zivoty<this.zivotypred)
		{
			this.dobit=0;
			this.stitpred=this.stit;
			this.zivotypred=this.zivoty;
		}
		else if(this.dobit<400)
			this.dobit++;
		else
		{
			this.stit+=this.maxstit/600;
			if(this.stit>this.maxstit)
				this.stit=this.maxstit;
			this.stitpred=this.stit;
		}
		//funkcia pasívneho spomaľovania lode
		if(this.vx>0)
			this.vx-=0.01;
		if(this.vy>0)
			this.vy-=0.01;
		if(this.vx<0)
			this.vx+=0.01;
		if(this.vy<0)
			this.vy+=0.01;
		//ovládanie
		if(keys[39]==true)
			this.otocenie+=2.5;
		if(keys[37]==true)
			this.otocenie-=2.5;
		if(this.otocenie>360)
			this.otocenie-=360;
		if(this.otocenie<0)
			this.otocenie+=360;
		
		//turbo
		if(keys[32]==true && this.stit>0)
		{
			if(zvukyon==1 && this.zvuk==0)
			{
				this.track.volume=1;
				this.track.loop=true;
				this.track.play();
				this.zvuk=1;
			}
			this.stit-=0.5-this.turbo/4;
			//zmena textury lode
			if(this.texture<6)
				this.texture=6;
			if(this.texture<7)
			{
				this.texture_timer++;
				if(this.texture_timer>80)
				{
					this.texture_timer=0;
					this.texture++;
				}
			}
			if(this.otocenie>=0 && this.otocenie<=90)
				{
					if(this.vx<1.25+this.turbo)
						this.vx+=(this.otocenie*(1/90))*this.turbo*0.5;
					if(this.vy>-1.25-this.turbo)
						this.vy+=(-90/90+this.otocenie*(1/90))*this.turbo*0.5;
				}
			if(this.otocenie>90 && this.otocenie<=180)
				{
					if(this.vx<1.25+this.turbo)
						this.vx+=(90/90-(this.otocenie-90)*(1/90))*this.turbo*0.5;
					if(this.vy<1.25+this.turbo)
						this.vy+=((this.otocenie-90)*(1/90))*this.turbo*0.5;
				}
			if(this.otocenie>180 && this.otocenie<=270)
				{
					if(this.vx>-1.25-this.turbo)
						this.vx+=(-(this.otocenie-180)*(1/90))*this.turbo*0.5;
					if(this.vy<1.25+this.turbo)
						this.vy+=(90/90-(this.otocenie-180)*(1/90))*this.turbo*0.5;
				}
			if(this.otocenie>270 && this.otocenie<=361)
				{
					if(this.vx>-1.25-this.turbo)
						this.vx+=(-90/90+(this.otocenie-270)*(1/90))*this.turbo*0.5;
					if(this.vy>-1.25-this.turbo)
						this.vy+=(-(this.otocenie-270)*(1/90))*this.turbo*0.5;
				}
		}
		//pohon
		else if(keys[38]==true)
		{
			if(zvukyon==1 && this.zvuk==0)
			{
				this.track.volume=0.5;
				this.track.loop=true;
				this.track.play();
				this.zvuk=1;
			}
			//zmena textury lode
			if(this.texture==3)
				this.texture++;
			if(this.texture>5)
				this.texture--;
			if(this.texture<5)
			{
				this.texture_timer++;
				if(this.texture_timer>80)
				{
					this.texture_timer=0;
					this.texture++;
				}
			}
			if(this.otocenie>=0 && this.otocenie<=90)
				{
					if(this.vx<1+this.pohon)
						this.vx+=(this.otocenie*(1/90))*this.pohon*0.5;
					if(this.vy>-1-this.pohon)
						this.vy+=(-90/90+this.otocenie*(1/90))*this.pohon*0.5;
				}
			else if(this.otocenie>90 && this.otocenie<=180)
				{
					if(this.vx<1+this.pohon)
						this.vx+=(90/90-(this.otocenie-90)*(1/90))*this.pohon*0.5;
					if(this.vy<1+this.pohon)
						this.vy+=((this.otocenie-90)*(1/90))*this.pohon*0.5;
				}
			else if(this.otocenie>180 && this.otocenie<=270)
				{
					if(this.vx>-1-this.pohon)
						this.vx+=(-(this.otocenie-180)*(1/90))*this.pohon*0.5;
					if(this.vy<1+this.pohon)
						this.vy+=(90/90-(this.otocenie-180)*(1/90))*this.pohon*0.5;
				}
			else if(this.otocenie>270 && this.otocenie<=361)
				{
					if(this.vx>-1-this.pohon)
						this.vx+=(-90/90+(this.otocenie-270)*(1/90))*this.pohon*0.5;
					if(this.vy>-1-this.pohon)
						this.vy+=(-(this.otocenie-270)*(1/90))*this.pohon*0.5;
				}
		}
		else if(this.texture>3)
		{
			if(this.zvuk==1)
			{
				this.zvuk=0;
				this.track.pause();
			}
			this.texture_timer--;
			if(this.texture_timer<0)
			{
				this.texture_timer=60;
				this.texture--;
			}
		}
		//prispôsobenie velkosti y ku zmenenej textúre
		if(this.texture==3)
		{
			this.velkosty=71;
			this.texture_timer=0;
		}
		if(this.texture==4)
			this.velkosty=72;
		if(this.texture==5)
			this.velkosty=76;
		if(this.texture==6)
			this.velkosty=76;
		if(this.texture==7)
			this.velkosty=79;
		//logika okraja mapy
		if(this.x<-40)
			this.vx=1.5;
		if(this.y<-60)
			this.vy=1.5;
		if(this.x+this.velkostx>platno.width+40)
			this.vx=-1.5;
		if(this.y>platno.height-110)
			this.vy=-1.5;
		
		//cd primary zbrane
		if(this.cd1>0)
			this.cd1--;
		//cd sekundárky
		if(this.cd2>0)
			this.cd2--;
		//streľba
		var a;
		if(keys[17]==true && this.cd1==0)
			naboje.push(typy[this.zbran1]());
		if(keys[67]==true && this.cd2==0)
			if((a=typy[this.zbran2]())!=-1)
				naboje.push(a);
	}
}