package lake.animals;

public class Fish extends Animal {
	
	public Fish() {
		this.nitrogenProduction = 3;
		this.name = "Ryba";
		System.out.println("Do jazera pribudol/la: " + this.name);
	}
	
	public Fish(int age) {
		this.age = age;
		this.nitrogenProduction = 3;
		this.name = "Ryba";
		System.out.println("Do jazera pribudol/la: " + this.name);
	}
	
	protected void living(int temperature, int nitrogenLevel) {
		if(temperature > 30)
			this.health -= (int)(Math.random()*9);
		if(nitrogenLevel > 50)
			this.health -= (int)(Math.random()*60);
		else
			this.health += (int)(Math.random()*3);
		if(this.health > 100)
			this.health = 100;
		if(nitrogenLevel > 0)
			this.health--;
	}
	
	public Fish mate() {
		this.reproductionCooldown = 15 + (int)(Math.random()*5);
		Fish baby = new Fish(-10);
		baby.reproductionCooldown = 35 + (int)(Math.random()*5);
		return baby;
	}
}
