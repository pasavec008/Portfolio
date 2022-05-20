//globalne premenné
var platno;
var context;
var keys = {};	//ovládanie klávesnicou
var pauza=1;	//mód hry
var ktory=0;	//pre poradové čísla enemies
var ktora=0;	//pre poradové čísla allies
var interval;	//predstavuje počet fps
var asteroidy=[]; //pole enemy veci, pre ktore collision "on"
var naboje=[];	//pole ally veci, pre ktore collision "on", mimo spaceship
var textury =[];	//pole s vloženými textúrami
var zbrane =[];		//pole s vloženými textúrami zbraní
var vylepsenie =[1,1,1,0,0,0,0,1,1,1,0,0,0,0]; //pole zaznamenávajúce stav vylepšenia jednotlivých vecí
var ceny =[];
var click;
var x;			//súradnice myšky
var y;			//súradnice myšky
var enemiesplus=[0,0,0,0,0,0,0,0,0,0];
var vlna=0;
var zvukyon=1;
var hudbaon=1;
var hudba=new Audio("music/Floathing_by_Roman_Gerhat.mp3");
var pomocna=1;

//spustenie
function spust(){
	platno=document.getElementById("platno");
	platno.addEventListener("click", kliknutie);
	context=platno.getContext("2d");
	interval=setInterval(step, 1000 / 70);
	ceny_set();
	pauza=3;
}