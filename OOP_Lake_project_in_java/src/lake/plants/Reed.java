package lake.plants;

public class Reed extends Plant {

	public Reed() {
		this.health = 3;
		this.name = "trstina";
		this.reproductionCooldown = 5;
		System.out.println("Do jazera pribudol/la: " + this.name);
	}
	
	public Reed reproduce() {
		this.reproductionCooldown = 4 + (int)(Math.random()*4);
		Reed red = new Reed();
		red.reproductionCooldown = 7 + (int)(Math.random()*4);
		return red;
	}
}
