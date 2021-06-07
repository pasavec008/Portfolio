package lake.animals;

public class Turtle extends Animal {
	
	public Turtle() {
		this.nitrogenProduction = 5;
		this.name = "Korytnacka";
		System.out.println("Do jazera pribudol/la: " + this.name);
	}
	
	public Turtle(int age) {
		this.age = age;
		this.nitrogenProduction = 5;
		this.name = "Korytnacka";
		System.out.println("Do jazera pribudol/la: " + this.name);
	}
	
	protected void living(int temperature, int nitrogenLevel) {
		if(temperature > 50)
			this.health -= 7;
		if(nitrogenLevel > 100)
			this.health -= (int)(Math.random()*30);
		else
			this.health += (int)(Math.random()*6);
		if(this.health > 100)
			this.health = 100;
		if(nitrogenLevel > 0)
			this.health--;
		this.age-=0.5;
	}
	
	public Turtle mate() {
		this.reproductionCooldown = 45 + (int)(Math.random()*5);
		Turtle baby = new Turtle(-20);
		baby.reproductionCooldown = 70 + (int)(Math.random()*5);
		return baby;
	}
}
