#ifndef DATA_SOURCE_H
#define DATA_SOURCE_H

#include <vector>
#include <string>
using namespace std;

#include <rapidjson/document.h>
#include "ServerComm.h" 
#include "./data_src/EarthquakeUSGS.h"
#include "./data_src/Game.h"
#include "./data_src/Shakespeare.h"
#include "./data_src/GutenbergBook.h"
#include "./data_src/CancerIncidence.h"
#include "./data_src/ActorMovieIMDB.h"


namespace bridges{
/**
 * @brief This provides an API to various data sources used in BRIDGES.
 *
 * BRIDGES currently supports a few external datasets for use with BRIDGES
 * assignments: USGIS Earthquake Tweeet streaming data feed, IMDB (file),
 * Book metadata collection, IGN Game Data, Shakespear book/poem meta data.
 
 * Functions are provided that access a user specified number of data 
 * records; objects of the appropriate type are returned as a list.
 *
 */

	namespace DataSource {

		vector<Game> getGameData() {
			using namespace rapidjson;
			Document d;

							// request the game dataset and parse it 
			d.Parse(
				ServerComm::makeRequest(
						"http://bridgesdata.herokuapp.com/api/games",
						{"Accept: application/json"}).c_str()
			);

			const Value& D = d["data"];
			vector<Game> wrapper;
			for (SizeType i=0; i < D.Size(); i++) {
				const Value& V = D[i];
				const Value& G = V["genre"];

				vector<string> genre;
				for(SizeType j=0; j<G.Size(); j++) {
					genre.push_back(G[j].GetString());
				}
				wrapper.push_back(
					Game(	V["game"].GetString(),
							V["platform"].GetString(),
							V["rating"].GetDouble(),
							genre ) );
			}
			return wrapper;
		}

		vector<ActorMovieIMDB> getActorMovieIMDBData(int number = 0) {
			using namespace rapidjson;
			Document d;
			vector<ActorMovieIMDB> wrapper;
			string url = "https://bridgesdata.herokuapp.com/api/imdb";

										// retrieve the data and parse
			d.Parse(ServerComm::makeRequest( url,
				{"Accept: application/json"}).c_str());

			const Value& D = d["data"];
			for (SizeType i=0; i<D.Size(); i++) {
				const Value& V = D[i];
				wrapper.push_back(
					ActorMovieIMDB(
						V["actor"].GetString(),
						V["movie"].GetString()
					)
				);
			}
			return wrapper;
		}

		vector<ActorMovieIMDB> getActorMovieIMDBData2(int number = 0) {
			using namespace rapidjson;
			Document d;
			vector<ActorMovieIMDB> wrapper;
			string url = "https://bridgesdata.herokuapp.com/api/imdb2";

										// retrieve the data and parse
			d.Parse(ServerComm::makeRequest( url,
				{"Accept: application/json"}).c_str());

			const Value& D = d["data"];
			for (SizeType i=0; i<D.Size(); i++) {
				const Value& V = D[i];
				string actor = V["actor"].GetString();
				string movie = V["movie"].GetString();
				double rating = V["rating"].GetDouble();
				const Value& G = V["genres"];
				vector<string> genres;
				for(SizeType j = 0; j < G.Size(); j++) {
					genres.push_back(G[j].GetString());
				}
				wrapper.push_back(ActorMovieIMDB( actor, movie, rating, genres));
			}
			return wrapper;
		}

		vector<EarthquakeUSGS> getEarthquakeData(int number = 0) {
			using namespace rapidjson;
			Document d;
			vector<EarthquakeUSGS> wrapper;
			if(number <= 0) {
				d.Parse(ServerComm::makeRequest(
					"http://earthquakes-uncc.herokuapp.com/eq",
					{"Accept: application/json"}).c_str());
				for(SizeType i=0; i<d.Size(); i++) {
					const Value& V = d[i]["properties"];
					const Value& G = d[i]["geometry"]["coordinates"];
					wrapper.push_back(
						EarthquakeUSGS(
								V["mag"].GetDouble(),
								G[0].GetDouble(),
								G[1].GetDouble(),
								V["place"].GetString(),
								V["title"].GetString(),
								V["url"].GetString(),
								V["time"].GetString() ) 
					);
				}
			}
			else {
				d.Parse(ServerComm::makeRequest(
					"http://earthquakes-uncc.herokuapp.com/eq/latest/" +
					to_string(number),{"Accept: application/json"}).c_str());
				const Value& D = d["Earthquakes"];
				for(SizeType i=0; i<D.Size(); i++) {
					const Value& V = D[i]["properties"];
					const Value& G = D[i]["geometry"]["coordinates"];
		//				wrapper.push_back({V["mag"].GetDouble(),G[0].GetDouble(),G[1].GetDouble(),V["place"].GetString(),V["title"].GetString(),V["url"].GetString(),V["time"].GetString()});
					wrapper.push_back(
						EarthquakeUSGS(
							V["mag"].GetDouble(),
							G[0].GetDouble(),
							G[1].GetDouble(),
							V["place"].GetString(),
							V["title"].GetString(),
							V["url"].GetString(),
							V["time"].GetString() ) 
					);
				}
			}
			return wrapper;
		}
		/**
		 *	Valid endpoints: 'poems','plays', <title>
		 *	Valid queryParams: format{simple}
		 */
		vector<Shakespeare> getShakespeareData(string endpoint = "", 
									bool simple = false) {
			using namespace rapidjson;
			Document d;
			vector<Shakespeare> wrapper;
			string url = 
				"http://bridgesdata.herokuapp.com/api/shakespeare/"+endpoint;
			if(simple){
				url += "?format=simple";
			}
										// retrieve the data and parse
			d.Parse(ServerComm::makeRequest( url,
				{"Accept: application/json"}).c_str());

			const Value& D = d["data"];
			for (SizeType i=0; i<D.Size(); i++) {
				const Value& V = D[i];
				wrapper.push_back(
					Shakespeare(
						V["title"].GetString(),
						V["type"].GetString(),
						V["text"].GetString()
					)
				);
								
			}
			return wrapper;
		}

		/**
		 *
		 *	Valid endpoints: 'poems','plays', <title>
		 *	Valid queryParams: format{simple}
		 *
		 */
		vector<GutenbergBook> getGutenbergBookData(int num = 0) {
			using namespace rapidjson;

			Document d;
			vector<GutenbergBook> wrapper;
			string url = "http://bridgesdata.herokuapp.com/api/books";
			if (num > 0){
				url += "?limit="+to_string(num);
			}

			d.Parse(ServerComm::makeRequest(
				url,{"Accept: application/json"}).c_str());
			const Value& D = d["data"];
			for (SizeType i=0; i<D.Size(); i++) {
				const Value& V = D[i];

				const Value& A = V["author"];
				const Value& L = V["languages"];
					
				vector<string> lang;
				for(SizeType j=0; j<L.Size(); j++) {
					lang.push_back(L[j].GetString());
				}

				const Value& G = V["genres"];
				vector<string> genre;
				for (SizeType j=0; j<G.Size(); j++) {
					genre.push_back(G[j].GetString());
				}

				const Value& S = V["subjects"];
				vector<string> subject;
				for (SizeType j=0; j<S.Size(); j++) {
					subject.push_back(S[j].GetString());
				}

				const Value& M = V["metrics"];
				wrapper.push_back(
					GutenbergBook(
						A["name"].GetString(), 
						A["birth"].GetInt(),
						A["death"].GetInt(),
						V["title"].GetString(),
						lang, 
						genre, 
						subject,
						M["characters"].GetInt(),
						M["words"].GetInt(),
						M["sentences"].GetInt(),
						M["difficultWords"].GetInt(),
						V["url"].GetString(),
						V["downloads"].GetInt()
					)
				);
			}
			return wrapper;
		}
		/*
		 * Retrieves the CDC dataset into a vector of records
		 * See CancerIncidence class for more information
		 *
		 */
		vector<CancerIncidence> getCancerIncidenceData(int num = 0) {
			using namespace rapidjson;

			Document d;
			vector<CancerIncidence> wrapper;
			string url = 
				"https://bridgesdata.herokuapp.com/api/cancer/withlocations";
			if (num > 0){
				url += "?limit="+to_string(num);
			}

			d.Parse(ServerComm::makeRequest(
				url,{"Accept: application/json"}).c_str());

						// get the JSON dataset
			const Value& D = d["data"];

			CancerIncidence c;
			for (SizeType i = 0; i < D.Size(); i++) {
				const Value& v = D[i];
				const Value& age = v["Age"];
					
					c.setAgeAdjustedRate( age["Age Adjusted Rate"].GetDouble());
					c.setAgeAdjustedCI_Lower(age["Age Adjusted CI Lower"].GetDouble());
					c.setAgeAdjustedCI_Upper(age["Age Adjusted CI Upper"].GetDouble());
					
				c.setYear(v["Year"].GetInt());

				const Value& data = v["Data"];
					c.setCrudeRate(data["Crude Rate"].GetDouble());
					c.setCrudeRate_CI_Lower(data["Crude CI Lower"].GetDouble());
					c.setCrudeRate_CI_Upper(data["Crude CI Upper"].GetDouble());
					c.setRace(data["Race"].GetString());
					c.setPopulation(data["Population"].GetInt());
					c.setEventType(data["Event Type"].GetString());

				c.setAffectedArea(v["Area"].GetString());

				const Value& loc = v["loc"];
					c.setLocationX(loc[0].GetDouble());
					c.setLocationY(loc[1].GetDouble());

				wrapper.push_back(c);
			}
			return wrapper;
		}

	} // namespace DataSource

} // namespace bridges
#endif
