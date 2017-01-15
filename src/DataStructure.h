#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

/*! \mainpage BRIDGES
 *
 * \section overview_sec Overview
 *  The Bridging Real-world Infrastructure Designed to Goal-align, Engage, and
 *  Stimulate (BRIDGES) project is directed at improving the retention of sophomore
 *  students in Computer Science by (1) introducing real-world datasets (Facebook,
 *  Twitter, etc) into course projects involving computer science data structures,
 *  and (2) facilitating peer mentoring of sophomores by senior CS students in
 *  shared lab experiences that involve the use of the BRIDGES infrastructure in
 *  software development.
 *  \section br_client BRIDGES Client Design
 *  BRIDGES client side design loosely follows the basic data structure elements
 *  implemented and described in ``A Practical Introduction to Data Structures and
 *  Algorithm Analysis" by C.A. Shaffer (http://people.cs.vt.edu/shaffer/Book/).
 *  These elements are augmented to contain visual properties that are controlled
 *  by the user to customize the visual representation of the constructed data structure.
 *  Once a a data structure is ready to be visualized, related BRIDGES server calls are
 *  made to send a reprsentation of the data structure to BRIDGES server.
 *  \section br_server BRIDGES Server Design.
 *  BRIDGES server implements a combination of technologies (MongoDB, Node,
 *  d3JS(visualization) to receive a data structure representation for visualization.
 *  These are largely transparent to the user and involves the user being directed to a
 *  web page for viewing the data structure. Attention has been paid to provide
 *  meaningful error messages to the user in case problems are encountered in the process.
 *  \section api_sec API Descriptions.
 *  See the accompanying pages for detailed description of the BRIDGES classes
 *  \subsection sponsor_sec Sponsorship/Funding.
 *  BRIDGES is an NSF TUES Project.
 *
 *  \section contacts_sec Contacts:
 *  Kalpathi Subramanian, krs@uncc.edu, Jamie Payton, payton@uncc.edu,
 *  Michael Youngblood, gmyoungblood@gmail.com, Robert Kosara, rkosara@tableau.com

 *  Department of Computer Science, The University of North Carolina at Charlotte,
 *  Charlotte, NC.
 */
#include <string> //string
using namespace std;

namespace bridges{

const string// string constants  for use in constructing JSON representation of the data structure
    QUOTE = "\"",
    COMMA = ",",
    COLON = ":",
    OPEN_CURLY = "{",
    CLOSE_CURLY = "}",
    OPEN_BOX = "[",
    CLOSE_BOX = "]",
    OPEN_PARENS = "(",
    CLOSE_PARENS = ")";

constexpr int MAX_ELEMENTS_ALLOWED = 5000;// Maximum number of elements that can be visualized

//Forward Declaration for Befriendment
namespace Bridges{void visualize();}

/**
 * @brief This is the superclass of all data structure types in BRIDGES
 *
 * This is the superclass of all data structure types in BRIDGES.
 * Both Elements and Graphs inherit from this.
 *
 * @date 6/11/15, Dakota Carmer, Kalpathi Subramanian, 7/10/16
 */
class DataStructure {
			//Used for access to getDataStructureRepresentation()
	friend void Bridges:: visualize(); 

	public:
		/** Virtual Destructor */
		virtual ~DataStructure()=default;
		/** @return The string representation of this data structure type */
		virtual const string getDStype() const=0;
		/**
		 * Ease of use function for the deletion of an entire datastructure.
		 * Overrides should call delete on itself and each linked data structure
		 *
		 * @warning Only call if all these data structures were all 
		 *		dynamicaly allocated(aka: using new)
         */
        virtual void cleanup() {delete this;}

	private:
		/**
		 * Gets the JSON representation of this DataStructure's nodes and links
		 *
		 * @param arr_size The size of the array determined by this
		 * @return A pair holding the nodes and links JSON strings respectively
		 */
		virtual const pair<string,string> getDataStructureRepresentation() 
											const = 0;
};  //end of DataStructure class

}   //end of bridges namespace
#endif
