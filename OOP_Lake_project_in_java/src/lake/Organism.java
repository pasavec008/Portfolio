package lake;

public interface Organism {
	public int lifeCycle(int i, int temperature);
	public String getName();
	public int getHealth();
	public int getReproductionCooldown();
}
