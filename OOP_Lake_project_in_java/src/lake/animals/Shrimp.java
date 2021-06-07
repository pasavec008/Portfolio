package lake.animals;

public class Shrimp extends Animal {
	
	public Shrimp() {
		this.nitrogenProduction = -2;
		this.name = "kreveta";
		System.out.println("Do jazera pribudol/la: " + this.name);
	}
	
	public Shrimp(int age) {
		this.age = age;
		this.nitrogenProduction = -2;
		this.name = "kreveta";
		System.out.println("Do jazera pribudol/la: " + this.name);
	}
	
	protected void living(int temperature, int nitrogenLevel) {
		if(temperature > 30)
			this.health -= (int)(Math.random()*10);
		if(nitrogenLevel < 20)
			this.health -= (int)(Math.random()*50);
		else
			this.health += (int)(Math.random()*3);
		if(this.health > 100)
			this.health = 100;
	}
	
	public Shrimp mate() {
		this.reproductionCooldown = 10 + (int)(Math.random()*5);
		Shrimp shrimp = new Shrimp(-10);
		shrimp.reproductionCooldown = 15 + (int)(Math.random()*5);
		return shrimp;
	}
	
}
