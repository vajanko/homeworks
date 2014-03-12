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
		private static final int SEARCHES = 10;
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
				} else {
					// Print the measurement result.
					System.out.printf ("%8s %8d %8d %8d %8d %13d %13d\n", method, ++attempt, GRAPH_NODES, GRAPH_EDGES, iDistance, iTime / 1000, iTTime / 1000);
				}
			}
		}
	
	public static void main(String[] args) {
		try
		{
			System.out.printf ("%8s %8s %8s %8s %13s %13s\n", "Attempt", "Nodes", "Edges", "Distance", "Time", "TTime");
			
			for (int i = 2; i < 5; i++)
			{
				transDistance = i;
				System.out.println("Distance: " + transDistance);
			
			// A)
			oSearcher = new SearcherImpl ();
			// each option gets the same graphs
			oRandom = new Random(SEED);
			attempt = 0;
			method = "LSLN";
			
			// Create a randomly connected graph and do a quick measurement.
			for (int edges = GRAPH_NODES; edges < GRAPH_NODES * 20; edges += GRAPH_NODES)
			{
				GRAPH_EDGES = edges;
				
				createNodes (GRAPH_NODES);
				connectSomeNodes (GRAPH_EDGES);
				searchBenchmark (SEARCHES);
			}
			
			// B)
			oSearcher = (Searcher) Naming.lookup ("//localhost/searcher");
			// each option gets the same graphs
			oRandom = new Random(SEED);
			attempt = 0;
			method = "RSLN";
			
			// Create a randomly connected graph and do a quick measurement.
			for (int edges = GRAPH_NODES; edges < GRAPH_NODES * 20; edges += GRAPH_NODES)
			{
				GRAPH_EDGES = edges;
				
				createNodes (GRAPH_NODES);
				connectSomeNodes (GRAPH_EDGES);
				searchBenchmark (SEARCHES);
			}
			
			// C)
			oSearcher = new SearcherImpl();
			oGenerator = (GraphGenerator) Naming.lookup("//localhost/generator");
			// each option gets the same graphs
			oRandom = new Random(SEED);
			attempt = 0;
			method = "LSRN";
			
			// Create a randomly connected graph and do a quick measurement.
			for (int edges = GRAPH_NODES; edges < GRAPH_NODES * 20; edges += GRAPH_NODES)
			{
				GRAPH_EDGES = edges;
				
				createRemoteNodes (GRAPH_NODES);
				connectSomeNodes (GRAPH_EDGES);
				searchBenchmark (SEARCHES);
			}
			
			// C)
			oSearcher = (Searcher) Naming.lookup ("//localhost/searcher");
			oGenerator = (GraphGenerator) Naming.lookup("//localhost/generator");
			// each option gets the same graphs
			oRandom = new Random(SEED);
			attempt = 0;
			method = "RSRN";
			
			// Create a randomly connected graph and do a quick measurement.
			for (int edges = GRAPH_NODES; edges < GRAPH_NODES * 20; edges += GRAPH_NODES)
			{
				GRAPH_EDGES = edges;
				
				createRemoteNodes (GRAPH_NODES);		// create object from client on the server
				connectSomeNodes (GRAPH_EDGES);
				searchBenchmark (SEARCHES);
			}
			
			}
		}
		catch (Exception e)
		{ 
			System.out.println ("Client Exception: " + e.getMessage ());
			e.printStackTrace ();
		}

	}
}
