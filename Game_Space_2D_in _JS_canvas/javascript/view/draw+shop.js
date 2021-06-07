function draw()
		{
		// Clear canvas
			context.clearRect(0, 0, platno.width, platno.height);
		//pozadie
			context.drawImage(textury[pozadie.texture],0,0);
		//render each asteroid
			var vyhra=0;
			for (var i in asteroidy)
			{
				var objekt = asteroidy[i];
				otoc_vykresli(objekt);
				vyhra++;
			}	
		//render each naboj
			for (var i in naboje)
			{
				var objekt = naboje[i];
				otoc_vykresli(objekt);
			}
		//render spaceship
			otoc_vykresli(spaceship);
			if(spaceship.stit>0)
			{	
				stit.x=spaceship.x-10;
				stit.y=spaceship.y-10;
				stit.otocenie=spaceship.otocenie;
				otoc_vykresli(stit);
				if(spaceship.timer>0)
					spaceship.timer--;
				else
					stit.texture=13;
			}
			
		//hud
			hud();
		
		//kontrolujem, či neposlať ďalšiu vlnu enemies

			if(vyhra<=vlna)
			{
				if(enemiesplus[0]>0)
				{
					asteroidy.push(asteroid());
					enemiesplus[0]--;
				}
				if(enemiesplus[1]>0)
				{
					asteroidy.push(venom());
					enemiesplus[1]--;
				}
				if(enemiesplus[2]>0)
				{
					asteroidy.push(abom());
					enemiesplus[2]--;
				}
				if(enemiesplus[3]>0)
				{
					asteroidy.push(krystal());
					enemiesplus[3]--;
				}
				if(enemiesplus[4]>0)
				{
					asteroidy.push(veteran());
					enemiesplus[4]--;
				}
				
				
				
				if(enemiesplus[5]>0)
				{
					asteroidy.push(asteroid());
					enemiesplus[5]--;
				}
				else if(enemiesplus[6]>0)
				{
					asteroidy.push(venom());
					enemiesplus[6]--;
				}
				else if(enemiesplus[7]>0)
				{
					asteroidy.push(abom());
					enemiesplus[7]--;
				}
				else if(enemiesplus[8]>0)
				{
					asteroidy.push(krystal());
					enemiesplus[8]--;
				}
				else if(enemiesplus[9]>0)
				{
					asteroidy.push(veteran());
					enemiesplus[9]--;
				}
			}
		
		//kontrola, či som neporazil všetkých nepriateľov
			if(vyhra==0 && pomocna==2)
			{
				spaceship.track.pause();
				setTimeout(function(){pauza=2;spaceship.score+=3000;}, 2000);
				pomocna=1;
				if(spaceship.uroven<spaceship.akturoven)
					spaceship.uroven=spaceship.akturoven;
			}
		}
		
//shopmenu
function shopmenu()
{
	context.clearRect(0, 0, platno.width, platno.height);
	var i;
	var q;
	var j;
	context.drawImage(textury[21],0,0);
	currency(465,22,spaceship.score);
	for(q=0;q<2;q++)
	{
		for(i=0;i<7;i++)
		{
			for(j=0;j<vylepsenie[i+q*7];j++)
				context.drawImage(textury[22+q],126+j*15+700*q,213+68*i);
		}
	}
	//popisky
	for(q=0;q<2;q++)
	{
		for(i=0;i<7;i++)
		{
			//vykresli MAX
			if(vylepsenie[i+q*7]==7)
				context.drawImage(textury[102],29+q*700,175+i*68);
			if(x>29+q*700 && x<265+q*700 && y>175+i*68 && y<231+i*68)
			{
			
				context.drawImage(textury[37+i+q*7],310,568);	//vykresli tooltip
				if(vylepsenie[i+q*7]<7)							//vypíš cenu
					currency(495,740,ceny[i+q*7]);
					
				else if(vylepsenie[i+q*7]>=7 || ceny[i+q*7]>spaceship.score)
					context.drawImage(textury[53],445,722);
				
				if(click==true && ceny[i+q*7]<=spaceship.score && vylepsenie[i+q*7]<7 && (x<126+q*700 || x>227+q*700 || y<184+i*68 || y>203+i*68))
				{
					spaceship.score-=ceny[i+q*7];
					vylepsenie[i+q*7]++;
					click=false;
					ceny_set();
				}
				
				if(x<126+q*700 || x>227+q*700 || y<184+i*68 || y>203+i*68);
				else if(click==true && vylepsenie[i+q*7]>0 && i+q*7!=0 && i+q*7!=1 && i+q*7!=7 && i+q*7!=8)
				{
					if(q==0)
						spaceship.zbran1=2*i-4;
					else
						spaceship.zbran2=2*i-3;
					click=false;
				}
			}
			if(i>1)
				{
					if((spaceship.zbran1==2*i-4 && q==0) || (spaceship.zbran2==2*i-3 && q==1))
						context.drawImage(textury[51],126+q*700,320+(i-2)*68);
					else
						context.drawImage(textury[52],126+q*700,320+(i-2)*68);
				}
		}
	}
	//hudbaon
	if(x>=5 && x<=63 && y>=6 && y<=51 && click==true)
		{
			hudbaon*=-1;
			click=false;
		}
	if(hudbaon==1)
		context.drawImage(textury[100],10,10);
	else
		context.drawImage(textury[101],10,10);
	if(hudbaon==1)
		hudba.play();
	else
		hudba.pause();
	
	//zvukyon
	if(x>=73 && x<=131 && y>=6 && y<=51 && click==true)
		{
			zvukyon*=-1;
			click=false;
		}
	if(zvukyon==1)
		context.drawImage(textury[68],80,10);
	else
		context.drawImage(textury[69],80,10);
	
	
	//lvls vykreslenie
	context.drawImage(textury[92+spaceship.uroven],337,116);
	
	//lvls voľba
	if(x>=460 && x<=460+44 && y>=116 && y<=116+45 && click==true)
		{
			level_1();
			click=false;
		}
	if(x>=532 && x<=532+44 && y>=183 && y<=183+45 && click==true && spaceship.uroven>=1)
		{
			level_2();
			click=false;
		}
	if(x>=470 && x<=470+44 && y>=252 && y<=252+45 && click==true && spaceship.uroven>=2)
		{
			level_3();
			click=false;
		}
	if(x>=531 && x<=531+44 && y>=331 && y<=331+45 && click==true && spaceship.uroven>=3)
		{
			level_4();
			click=false;
		}
	if(x>=459 && x<=459+44 && y>=375 && y<=375+45 && click==true && spaceship.uroven>=4)
		{
			level_5();
			click=false;
		}
	if(x>=436 && x<=436+44 && y>=472 && y<=472+45 && click==true && spaceship.uroven>=5)
		{
			level_6();
			click=false;
		}
	if(x>=340 && x<=346+50 && y>=504 && y<=510+49 && click==true && spaceship.uroven>=6)
		{
			level_7();
			click=false;
		}
	
}
//funkcia na aktualizovanie cien po kúpe
function ceny_set()
{
	ceny[0] =500+1000*vylepsenie[0];
	ceny[1] =500+750*vylepsenie[1];
	ceny[2] =500+1000*vylepsenie[2];
	ceny[3] =1200+1200*vylepsenie[3];
	ceny[4] =2000+1500*vylepsenie[4];
	ceny[5] =2500+1200*vylepsenie[5];
	ceny[6] =4000+1500*vylepsenie[6];
	ceny[7] =1000+1000*vylepsenie[7];
	ceny[8] =750+750*vylepsenie[8];
	ceny[9] =1000+1000*vylepsenie[9];
	ceny[10] =2000+1200*vylepsenie[10];
	ceny[11] =2500+1300*vylepsenie[11];
	ceny[12] =3000+1500*vylepsenie[12];
	ceny[13] =4000+2000*vylepsenie[13];
}

function zaciatok()
{
	context.clearRect(0, 0, platno.width, platno.height);
	if(pomocna==1)
	{
		context.drawImage(textury[66],0,0);
		if(x>=268 && x<=300 && y>=620 && y<=659 && click==true)
		{
			hudbaon*=-1;
			click=false;
		}
		if(x>=408 && x<=588 && y>=451 && y<=512 && click==true)
		{
			pomocna=2;
			click=false;
		}
		if(hudbaon==1)
			context.drawImage(textury[100],260,620);
		else
			context.drawImage(textury[101],260,620);	
		
		//zvukyon
		if(zvukyon==1)
			context.drawImage(textury[68],320,620);
		else
			context.drawImage(textury[69],320,620);
		
		if(x>=320 && x<=370 && y>=620 && y<=659 && click==true)
		{
			zvukyon*=-1;
			click=false;
		}
	}
	if(pomocna==2)
	{
		context.drawImage(textury[67],0,0);
		if(x>=408 && x<=588 && y>=179 && y<=240 && click==true)
		{
			level_1();
			pauza=1;
		}
	}
}





