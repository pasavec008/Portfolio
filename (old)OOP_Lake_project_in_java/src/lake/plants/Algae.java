package lake.plants;

public class Algae extends Plant {

	public Algae() {
		this.name = "riasa";
		this.reproductionCooldown = 2;
		System.out.println("Do jazera pribudol/la: " + this.name);
	}
	
	public Algae reproduce() {
		this.reproductionCooldown = 1 + (int)(Math.random()*5);
		Algae alg = new Algae();
		alg.reproductionCooldown = 3 + (int)(Math.random()*5);
		return alg;
	}
}
