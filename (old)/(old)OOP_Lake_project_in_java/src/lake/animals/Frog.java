package lake.animals;

public class Frog extends Animal {
	
	public Frog() {
		this.name = "Zaba";
		System.out.println("Do jazera pribudol/la: " + this.name);
	}
	
	public Frog(int age) {
		this.age = age;
		this.name = "Zaba";
		System.out.println("Do jazera pribudol/la: " + this.name);
	}
	
	protected void living(int temperature, int nitrogenLevel) {
		if(temperature > 35)
			this.health -= (int)(Math.random()*8);
		if(nitrogenLevel > 75)
			this.health -= (int)(Math.random()*50);
		else
			this.health += (int)(Math.random()*3);
		if(this.health > 100)
			this.health = 100;
		if(nitrogenLevel > 0)
			this.health--;
	}
	
	public Frog mate() {
		this.reproductionCooldown = 25 + (int)(Math.random()*5);
		Frog baby = new Frog(-10);
		baby.reproductionCooldown = 45 + (int)(Math.random()*5);
		return baby;
	}
}
