package elevator;

/*
 * Copyright (C) 2000 by ETHZ/INF/CS
 * All rights reserved
 * 
 * @version $Id$
 * @author Roger Karrer
 */

import java.util.List;
import java.util.ArrayList;

// class to represent a floor in the control object
class Floor {

    // Lists of people waiting to go up, and down
    // The Vectors will have instances of Integer objects.  The Integer will
    // store the floor that the person wants to go to
    public List upPeople, downPeople;

    // True if an elevator has claimed the the up or down call
    public boolean upFlag, downFlag;

    public Floor() {
        upPeople = new ArrayList();
        downPeople = new ArrayList();
        upFlag = false;
        downFlag = false;
    }
}
 
