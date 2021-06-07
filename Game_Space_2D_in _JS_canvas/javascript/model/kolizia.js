//funkcia zisťujúca kolíziu medzi dvoma objektami
function kolizia(objekt_1,objekt_2)
{
	//zisťujeme, či sa roh jedného objektu dostal do oblasti druhého
	if((objekt_1.x>=objekt_2.x && objekt_1.x<=objekt_2.x+objekt_2.velkostx && objekt_1.y>=objekt_2.y && objekt_1.y<=objekt_2.y+objekt_2.velkosty)||
	(objekt_1.x+objekt_1.velkostx>=objekt_2.x && objekt_1.x+objekt_1.velkostx<=objekt_2.x+objekt_2.velkostx && objekt_1.y>=objekt_2.y && objekt_1.y<=objekt_2.y+objekt_2.velkosty)||
	(objekt_1.x>=objekt_2.x && objekt_1.x<=objekt_2.x+objekt_2.velkostx && objekt_1.y+objekt_1.velkosty>=objekt_2.y && objekt_1.y+objekt_1.velkosty<=objekt_2.y+objekt_2.velkosty)||
	(objekt_1.x+objekt_1.velkostx>=objekt_2.x && objekt_1.x+objekt_1.velkostx<=objekt_2.x+objekt_2.velkostx && objekt_1.y+objekt_1.velkosty>=objekt_2.y && objekt_1.y+objekt_1.velkosty<=objekt_2.y+objekt_2.velkosty))
	{
		objekt_1.special();
		objekt_2.special();
		if(objekt_1.zivoty+objekt_1.stit<objekt_2.zivoty+objekt_2.stit)
		{
			if(objekt_2.stit==0)
			{
				objekt_2.zivoty-=(objekt_1.maxzivoty+objekt_1.stit);
			}
			else
			{
				objekt_2.stit-=(objekt_1.maxzivoty+objekt_1.stit)/4*3;
				if(objekt_2.stit<0)
				{
					objekt_2.zivoty+=objekt_2.stit;
					objekt_2.stit=0;
				}
				objekt_2.zivoty-=(objekt_1.maxzivoty+objekt_1.stit)/4;
			}
			objekt_1.zivoty=0;
		}
		
		if(objekt_2.zivoty+objekt_2.stit<objekt_1.zivoty+objekt_1.stit)
		{
			if(objekt_1.stit==0)
			{
				objekt_1.zivoty-=(objekt_2.maxzivoty+objekt_2.stit);
			}
			else
			{
				objekt_1.stit-=(objekt_2.maxzivoty+objekt_2.stit)/4*3;
				if(objekt_1.stit<0)
				{
					objekt_1.zivoty+=objekt_1.stit;
					objekt_1.stit=0;
				}
				objekt_1.zivoty-=(objekt_2.maxzivoty+objekt_2.stit)/4;
			}
			objekt_2.zivoty=0;
		}
		return 1;
	}
	return 0;
}