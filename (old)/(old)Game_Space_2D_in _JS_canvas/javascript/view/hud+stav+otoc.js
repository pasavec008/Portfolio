//funkcia vykreslujúca počet meny
function currency(x,y,pom)
{
	var desat=1;
	var kolko=0;
	var i;
	while(Math.floor(pom/desat)!=0)
	{
		kolko++;
		desat*=10;
	}
	desat/=10;
	for(i=0;i<kolko;i++)
	{
		context.drawImage(textury[24],0+17*Math.floor(pom/desat),0,17,20,x+i*17,y,17,20);
		pom=pom%desat;
		desat/=10;
	}
}
//funkcia vykreslujúca lose screen
function prehra()
{
	if(pauza==-2)
		context.drawImage(textury[11],0,0);
	else
		context.drawImage(textury[103],0,0);
	currency(493,556,spaceship.score);
	if(x>=425 && x<=578 && y>=607 && y<=659 && click==true)
		{
			pauza=2;
			click=false;
		}
}


//funkcia na otočenie a vykreslenie objektu (treba velkost a suradnice a otocenie a texturu)
function otoc_vykresli(objekt) {
	context.save();
	context.translate(objekt.x+objekt.velkostx/2,objekt.y+objekt.velkosty/2);
	context.rotate(objekt.otocenie * Math.PI / 180.0);
	context.drawImage(textury[objekt.texture], 0-objekt.velkostx/2, 0-objekt.velkosty/2, objekt.velkostx, objekt.velkosty);
	context.restore();
}

//funkcia vykreslujuca HUDko, zivoty a stit
function hud(){
	context.drawImage(textury[8],0,697);
	
	//zivoty
	var i;
	if(spaceship.maxzivoty==spaceship.zivoty)
		i=30;
	else
	{
		i=spaceship.maxzivoty/30;
		i=Math.floor(spaceship.zivoty/i);
	}
	for(i;i>0;i--)
		context.drawImage(textury[9],582+5*i,741);
	//stit
	if(spaceship.maxstit==spaceship.stit)
		i=30;
	else
	{
		i=spaceship.maxstit/30;
		i=Math.floor(spaceship.stit/i);
	}
	for(i;i>0;i--)
		context.drawImage(textury[10],821+5*i,741);
		
	//primarna zbran + cd
	context.drawImage(zbrane[spaceship.zbran1],281,725);
	for(i=0;i<(31/spaceship.zbran1_cd*spaceship.cd1);i++)
		context.drawImage(textury[16],277+2*i,724);
	//sekundarka +cd
	context.drawImage(zbrane[spaceship.zbran2],375,725);
	for(i=0;i<(31/spaceship.zbran2_cd*spaceship.cd2);i++)
		context.drawImage(textury[16],371+2*i,724);
	//score
	currency(85,745,spaceship.score);
}