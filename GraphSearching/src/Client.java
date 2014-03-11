import java.rmi.Naming;
import java.rmi.RemoteException;
import java.util.Random;


public class Client {
	
	// How many nodes and how many edges to create.
		private static final int GRAPH_NODES = 1000;
		private static final int GRAPH_EDGES = 2000;
		
		// How many searches to perform
		private static final int SEARCHES = 50;
		
		private static Node [] aNodes;
		
		private static Random oRandom = new Random ();
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
			System.out.printf ("%7s %8s %13s %13s\n", "Attempt", "Distance", "Time", "TTime");
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
				int iTDistance = oSearcher.getTransitiveDistance (4, aNodes [iNodeFrom], aNodes [iNodeTo]);
				iTTime = System.nanoTime () - iTTime;
				
				if (iDistance != iTDistance)
				{
					System.out.printf("Standard and transitive algorithms inconsistent (%d != %d)\n", iDistance, iTDistance);
				} else {
					// Print the measurement result.
					System.out.printf ("%7d %8d %13d %13d\n", i, iDistance, iTime / 1000, iTTime / 1000);
				}
			}
		}
	
	public static void main(String[] args) {
		try
		{
			System.out.println("LOCAL SERACHER, LOCAL NODES");
			oSearcher = new SearcherImpl ();
			// Create a randomly connected graph and do a quick measurement.
			// Consider replacing connectSomeNodes with connectAllNodes to
			// verify that all distances are equal to one.
			createNodes (GRAPH_NODES);
			connectSomeNodes (GRAPH_EDGES);
			connectAllNodes();
			searchBenchmark (SEARCHES);
			
			
			System.out.println("REMOTE SEARCHER, LOCAL NODES");
			// Use the registry on this host to find the server.
			// The host name must be changed if the server uses
			// another computer than the client!
			oSearcher = (Searcher) Naming.lookup ("//localhost/searcher");
		
			// Create a randomly connected graph and do a quick measurement.
			// Consider replacing connectSomeNodes with connectAllNodes to
			// verify that all distances are equal to one.
			createNodes (GRAPH_NODES);
			connectSomeNodes (GRAPH_EDGES);
			searchBenchmark (SEARCHES);
			
			
			System.out.println("LOCAL SEARCHER, REMOTE NODES");
			// Use the registry on this host to find the server.
			// The host name must be changed if the server uses
			// another computer than the client!
			oSearcher = new SearcherImpl();
			oGenerator = (GraphGenerator) Naming.lookup("//localhost/generator");
		
			// Create a randomly connected graph and do a quick measurement.
			// Consider replacing connectSomeNodes with connectAllNodes to
			// verify that all distances are equal to one.
			/*createRemoteNodes (GRAPH_NODES);
			connectSomeNodes (GRAPH_EDGES);
			searchBenchmark (SEARCHES);*/
			
			
			System.out.println("REMOTE SEARCHER, REMOTE NODES");
			oSearcher = (Searcher) Naming.lookup ("//localhost/searcher");
			oGenerator = (GraphGenerator) Naming.lookup("//localhost/generator");
			
			createRemoteNodes (GRAPH_NODES);		// create object from client on the server
			//connectSomeNodes (GRAPH_EDGES);
			connectAllNodes();
			searchBenchmark (SEARCHES);
		}
		catch (Exception e)
		{ 
			System.out.println ("Client Exception: " + e.getMessage ());
			e.printStackTrace ();
		}

	}

}
