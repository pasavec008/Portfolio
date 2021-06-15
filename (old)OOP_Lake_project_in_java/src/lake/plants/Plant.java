package lake.plants;
import lake.Organism;

abstract public class Plant implements Organism {
	protected int age = 0;
	protected int health = 2;
	protected String name;
	protected int reproductionCooldown = (int)(Math.random()*5);
	
	private void aging() {
		this.age++;
		if(this.reproductionCooldown > 0)
			this.reproductionCooldown--;
	}
	
	public int lifeCycle(int nitrogenLevel, int temperature) {
		this.aging();
		if(nitrogenLevel < -(Math.random()*20))
			this.health--;
		if(nitrogenLevel > (Math.random()*100) && this.health < 3)
			this.health++;
		return -(this.health + 1);
	}
	
	public int getReproductionCooldown() {
		return this.reproductionCooldown;
	}
	
	public String getName() {
		return this.name;
	}
	
	public int getHealth() {
		return this.health;
	}
	
	abstract public Plant reproduce();
	
}
