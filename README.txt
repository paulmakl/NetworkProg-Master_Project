Hello Brad,

	One day late, but still awesome! We couldn't get CRC's to work but we got everything looking awesome! This semester was a challenge, but we ultimately did it. I thought I would talk a little bit about our implementation before you took a dive into our code.

	The first piece of our code that the layer above interacts with is the Demibrad class. Demibrad has two purposes, create a sender and a receiver thread, and to store important data for the threads and the layer above. Demibrad has several "outer" functions. These are available to the layer above. Once the layer above calls the "init" function, it creates two threads, one thread sends data whenever demibrad puts data in a shared queue, the other sends. Demibrad also updates.

	Packet is an interesting class. Packet data is stored in a packet class with all of the data that needs to be sent. There is one long function that puts all the data into a byte array called Build_Byte_Array. it has multiple constructors based on what type of packet is being created. The packet class does not handle beacons.

	The Listener class handles listening to the RF layer. It blocks using the receive method from the RF layer till it gets data. If the packet is for us, then it sends an ack to the sender and tells our own sender thread to stop waiting for an ack. The listener thread also handles beacons.

	The sender thread is complicated. It handles all the mac rules and handles retransmissions. 

	All threads have a set of mutexs so that we can avoid concurrent access problems. All threads also read and write to the ostream and update status codes based on what happens inside of them.


From:
	Paul
	Erin
	Weston