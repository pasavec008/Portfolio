package lake.animals;
import lake.Organism;

abstract public class Animal implements Organism{
	protected double age = 0;
	protected int health = 100;
	protected String name;
	protected int nitrogenProduction = 2;
	protected int reproductionCooldown = (int)(Math.random()*7) + 2;

	
	private void aging() {
		this.age++;
		if(this.reproductionCooldown > 0)
			this.reproductionCooldown--;
		if(this.age > 100)
			this.health -= (int)((this.age - 100)/3);
	}
	
	protected void living(int temperature, int nitrogenLevel) {
		
	}

	public int getHealth() {
		return this.health;
	}
	
	public int getReproductionCooldown() {
		return this.reproductionCooldown;
	}
	
	public String getName() {
		return this.name;
	}
	
	abstract public Animal mate();
	
	public int lifeCycle(int temperature, int nitrogenLevel) {
		this.aging();
		this.living(temperature, nitrogenLevel);
		return this.nitrogenProduction;
	}
	
}
