import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.util.Random;


public class Client {
	
	private static final long SEED = 100;	// use the same graph for all methods, also find distances between the same nodes
	
	// How many nodes and how many edges to create.
	private static final int GRAPH_NODES = 500;
	private static int GRAPH_EDGES = 2000;
	
	// How many searches to perform
	private static final int SEARCHES = 1;
	private static int attempt = 0;
	private static int transDistance = 4;
	private static String method;
	
	private static Node [] aNodes;
	
	private static Random oRandom = new Random();
	private static Searcher oSearcher = null;
	private static GraphGenerator oGenerator = null;
	
	/**
	 * Creates nodes of a graph.
	 * @param iHowMany
	 */
	public static void createNodes (int iHowMany)
	{
		aNodes = new Node [iHowMany];
		
		for (int i = 0 ; i < iHowMany ; i ++)
		{
			aNodes [i] = new NodeImpl ();
		}
	}
	/**
	 * Creates nodes of a graph.
	 * @param iHowMany
	 * @throws RemoteException 
	 */
	public static void createRemoteNodes(int iHowMany) throws RemoteException
	{
		aNodes = new Node[iHowMany];
		for (int i = 0; i < iHowMany; i++)
		{
			aNodes[i] = oGenerator.CreateNode();
		}
	}
	
	/**
	 * Creates a fully connected graph. 
	 * @throws RemoteException 
	 */
	public static void connectAllNodes () throws RemoteException
	{
		for (int iNodeFrom = 0 ; iNodeFrom < aNodes.length ; iNodeFrom++)
		{
			for (int iNodeTo = iNodeFrom + 1 ; iNodeTo < aNodes.length ; iNodeTo++)
			{
				aNodes [iNodeFrom].addNeighbor (aNodes [iNodeTo]);
				aNodes [iNodeTo].addNeighbor (aNodes [iNodeFrom]);
			}
		}
	}
	
	/**
	 * Creates a randomly connected graph.
	 * @param iHowMany
	 * @throws RemoteException 
	 */
	public static void connectSomeNodes (int iHowMany) throws RemoteException
	{
		for (int i = 0 ; i < iHowMany ; i ++)
		{
			int iNodeFrom = oRandom.nextInt (aNodes.length);
			int iNodeTo = oRandom.nextInt (aNodes.length);
			
			aNodes [iNodeFrom].addNeighbor (aNodes [iNodeTo]);
		}
	}
	
	/**
	 * Runs a quick measurement on the graph.
	 * @param iHowMany
	 * @throws RemoteException 
	 */
	public static void searchBenchmark (int iHowMany) throws RemoteException
	{
		// Display measurement header.
		//System.out.printf ("%8s %8s %8s %8s %13s %13s\n", "Attempt", "Nodes", "Edges", "Distance", "Time", "TTime");
		for (int i = 0 ; i < iHowMany ; i ++)
		{
			++attempt;
			// Select two random nodes.
			int iNodeFrom = oRandom.nextInt (aNodes.length);
			int iNodeTo = oRandom.nextInt (aNodes.length);
			
			// Calculate distance, timing the operation.
			long iTime = System.nanoTime ();
			
			int iDistance = oSearcher.getDistance (aNodes [iNodeFrom], aNodes [iNodeTo]);
			iTime = System.nanoTime () - iTime;
			
			long iTTime = System.nanoTime ();
			int iTDistance = oSearcher.getTransitiveDistance (transDistance, aNodes [iNodeFrom], aNodes [iNodeTo]);
			iTTime = System.nanoTime () - iTTime;
			
			if (iDistance != iTDistance)
			{
				System.out.printf("Standard and transitive algorithms inconsistent (%d != %d)\n", iDistance, iTDistance);
				System.out.printf("transDistance: %d\n", transDistance);
				System.out.printf("from: %d, to: %d", iNodeFrom, iNodeTo);
				System.exit(0);
				
			} else {
				// Print the measurement result.
				System.out.printf ("%8s %8d %8d %8d %8d %13d %13d\n", method, attempt, GRAPH_NODES, GRAPH_EDGES, iDistance, iTime / 1000, iTTime / 1000);
			}
		}
	}

	private static void runBenchmarks(boolean remoteNodes) throws RemoteException
	{
		// initialize test number counter
		attempt = 0;
		// always use the same graphs for all methods
		oRandom = new Random(SEED);
		
		// Create a randomly connected graph and do a quick measurement.
		for (int edges = GRAPH_NODES; edges < GRAPH_NODES * 20; edges += GRAPH_NODES)
		{
			GRAPH_EDGES = edges;
			
			if (remoteNodes)
				createRemoteNodes(GRAPH_NODES);
			else
				createNodes (GRAPH_NODES);
			
			connectSomeNodes (GRAPH_EDGES);
			searchBenchmark (SEARCHES);
		}
	}
	private static void runLSLN() throws RemoteException
	{
		oSearcher = new LSearcherImpl ();
		method = "LSLN";
		
		runBenchmarks(false);
	}
	private static void runRSLN(String hostname) throws MalformedURLException, RemoteException, NotBoundException
	{
		oSearcher = (Searcher) Naming.lookup (hostname + "/searcher");
		method = "RSLN";
		
		runBenchmarks(false);
	}
	private static void runLSRN(String hostname) throws MalformedURLException, RemoteException, NotBoundException
	{
		oSearcher = new LSearcherImpl();
		oGenerator = (GraphGenerator) Naming.lookup(hostname + "/generator");
		method = "LSRN";
		
		runBenchmarks(true);
	}
	private static void runRSRN(String hostname) throws MalformedURLException, RemoteException, NotBoundException
	{
		oSearcher = (Searcher) Naming.lookup (hostname + "/searcher");
		oGenerator = (GraphGenerator) Naming.lookup(hostname + "/generator");
		method = "RSRN";
		
		runBenchmarks(true);
	}
	private static void runAll(String hostname) throws MalformedURLException, RemoteException, NotBoundException
	{
		runLSLN();
		runRSLN(hostname);
		runLSRN(hostname);
		runRSRN(hostname);
	}
	
	public static void main(String[] args) {
		try
		{
			String hostname = "//localhost";
			if (args.length == 1)
				hostname = args[0];
			
			for (int i = 2; i < 5; i++)
			{
				transDistance = i;
				
				System.out.println("Transitive distance: " + transDistance);
				System.out.printf ("%8s %8s %8s %8s %8s %13s %13s\n", "Method", "Attempt", "Nodes", "Edges", "Distance", "Time", "TTime");
				
				runAll(hostname);
			}
			
			
		}
		catch (Exception e)
		{ 
			System.out.println ("Client Exception: " + e.getMessage ());
			
			e.printStackTrace ();
		}
	}
}
