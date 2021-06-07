package lake.animals;

public class Snail extends Animal {

	public Snail() {
		this.nitrogenProduction = -1;
		this.name = "Slimak";
		System.out.println("Do jazera pribudol/la: " + this.name);
	}
	
	public Snail(int age) {
		this.age = age;
		this.nitrogenProduction = -1;
		this.name = "Slimak";
		System.out.println("Do jazera pribudol/la: " + this.name);
	}
	
	protected void living(int temperature, int nitrogenLevel) {
		if(temperature > 25)
			this.health -= (int)(Math.random()*10);
		if(nitrogenLevel < 15)
			this.health -= (int)(Math.random()*50);
		else
			this.health += (int)(Math.random()*3);
		if(this.health > 100)
			this.health = 100;
	}
	
	public Snail mate() {
		this.reproductionCooldown = 7 + (int)(Math.random()*5);
		Snail baby = new Snail(-10);
		baby.reproductionCooldown = 12 + (int)(Math.random()*5);
		return baby;
	}

}
