#include <string>
#include <vector>
#include <functional>
#include <typeindex>

#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;


////////Handy objects

struct bool2 {
public:
		bool a, b;

		bool2(bool a, bool b) : a(a), b(b) {}
};

enum class ActionType
{
		MakeFriend = 1,
		Quarrel = 2,
		BecomeServant = 3,
		MakeServant = 4,
		Conspire = 5
};

enum class Relationship {
		None = 0,
		Friend = 1,
		Servant = 2,
		Lord = 3
};



class Condition {
public:
		Condition(int stPoint) : stPoint(stPoint)
		{}
		virtual ~Condition() {}
		virtual int checkMatchingResultCondition(Condition& cond) = 0;
		virtual Condition* makeConditionDynamic(int x) = 0;
		int stPoint; //zachem ustraivat' predpolozhenie dlya togo, kogda eshchyo ne yasno, yavlyaetsya li drugom voobshchexd
};

class ConditionGroup {
public:
		//more often one
		vector<Condition*> conds;

		//Xor vryad li budet ispol'zovat'sya, a And yavlyaetsya po-umolchaniyu
		enum GroupType { And, Or, Xor } groupType;

		int variationRequirementCreationMainNode;

		ConditionGroup() {};

		ConditionGroup(Condition* condition) { //Inicializirovat' ConditionGroup s odnim elementom
				conds.push_back(condition);
				groupType = And;
		}
		ConditionGroup(GroupType type) : groupType(type) {}
};

class Courtier {
public:
	vector<Relationship> relation;

	Courtier(int N) {

		relation.reserve(N);

		for (int i = 0; i < N; i++)
		{
			//In default None
			relation.push_back(Relationship::None);
		}
	}
};

class MainNode;
class Decision {
public:
	string name;
	char sign;
	
	vector<Condition*> result;

	//Eta funkciya vozvrashchaet ne vybrannyh dvornikov, a ostatki trebovanij. \
	Konkretnaya kombinaciya pridvornyh yavlyaetsya otvetstvennost'yu central'noj funkcii.
	function < pair<bool, std::vector<ConditionGroup>>(const vector<Courtier>& data, const int& x, const MainNode& node) > EvaluateCondition;


	Decision(string name, vector<Condition*> result,
		function < pair<bool, vector<ConditionGroup>>(const vector<Courtier>& data, const int& x, const MainNode& node) > function)
		: result(result), EvaluateCondition(function), name(name) {}


	bool operator ==(const Decision& decision) {
		return name == decision.name;
	}

};


/// <summary>
/// Global Variables
/// </summary>
static int last_court; //length - 1
static int first_court = 0;
static int N;
static int K;
static vector<Courtier> courtiers;
vector<Decision*> decisions;
vector<Decision*> assumptionDecisions = { new Decision("Quarrel", vector<Condition*>(), NULL) }; //Kogda K = N - 1, assumptions ne trebuyutsya.




class ConditionElement : public Condition {
public:
	Relationship relation;
	int A, B; // (dlya statichnoy informacii ne svyazannoj s courtiers) -1=first_court. -2=last_court. -3=x. -4=y. -5=z. -6=w ...\
	vybrany otricatel'nye chisla, chtoby pri nepravil'nom primenenii smogla by vydast'sya oshibka.

	ConditionElement(Relationship relation, int A, int B, int stPoint) : relation(relation), A(A), B(B), Condition(stPoint) {}

	int checkMatchingResultCondition(Condition& cond) {

		if (typeid(cond).hash_code() == typeid(ConditionElement).hash_code()) {

			ConditionElement* element = (ConditionElement*)&cond;

			if (B == element->B) { //Esli tak, to znachit oni kandidaty k tomu chtoby kollajdit' drug s drugom
				//Dopuskaetsya, chto v aktivnyh ConditionElement, A - eto vsegda last_court

				if (relation != element->relation)
					return -1; //Tak kak lyuboe drugoe otnoshenie zaslonyaet imeyushcheesya, oni odnoznachno protivorechat drug drugu - contradict-alternate
				else
					return 1; //Ravny, znachit vsyo raduzhno! Oni rodstvenny.
			}
			else return 0; //Esli x pervogo != x vtorogo, to oni nikak ne peresekayutsya, neutral

		}
		else
		{
			return 0; //neutral
		}
	}

	Condition* makeConditionDynamic(int x) {

		return new ConditionElement(relation, last_court, x, stPoint);
	}
};
class ValueCondition : public Condition {
public:
	int A, B;

	enum RelationType { Greater, Equal } state;

	ValueCondition(RelationType type, int A, int B, int stPoint) : state(type), A(A), B(B), Condition(stPoint) {}


	int checkMatchingResultCondition(Condition& cond) {

		//V etom sluchae uzhe ne tol'ko sam ValueCondition sobiraetsya byt' v dele) Eshchyo i Aspiration. //NO

		if (typeid(cond) == typeid(ValueCondition)) {

			ValueCondition* element = (ValueCondition*)&cond;

			if (B == element->B and A == element->A) {

				if (state != element->state)	return -1; //Tak kak lyuboe drugoe otnoshenie zaslonyaet imeyushcheesya, oni odnoznachno protivorechat drug drugu - contradict-alternate
				else							return 1; //Ravny, znachit vsyo raduzhno! Oni rodstvenny.
			}
			else if (A == element->B and B == element->A) { //Zdes' naoborot, uchityvaya perevyornutye znacheniya, chtoby byt' podhodyashchimi, ih state ne dolzhny sovpadat'

				if (state == element->state)	return -1;
				else							return 1;
			}
			else return 0;

		}
		else
		{
			return 0; //neutral
		}
	}

	//emmmm, a kak eto sdelat' takim))) Kak Value Condition mozhet stat' dinamicheskim) Prosto budet vozvrashchat' samogo sebya.
	Condition* makeConditionDynamic(int x) {
		return this;
	}
};


class DecisionData {
public:
		Decision* decision;

		int appliedToCourtier;

		DecisionData(Decision* decision, int appliedToCourtier) : decision(decision), appliedToCourtier(appliedToCourtier) {}
};


class MainNode {
public:

	int parentNode; //ns vector vse-taki...
	int derivedGroupsCount; //Ekvivalentno kolichestvu trebovanij, neobhodimyh dlya realizacii tekushchego MainNode. Dochernie ob"ekty budut ssylats'ya na\
								svoego roditelya, chtoby izmenyat' svyazannoe s nim chislo.
	int realizedGroupsCount; //Vot i samo chislo kotoroe budet uvelichit'sya. Kak tol'ko ono realizovano (ravno derivedGroupsCount), \
							 ,ono pustit signal svoemu sobstvennomu roditelyu o tom, chtoby ono tozhe okazalos' uchteno, i tak na protyazhenii vsej ierarhii. \
							Kak tol'ko realizedGroupsCount stanet ravnym derivedGroupsCount v pervom zvene, algoritm poschitaetsya vypolnenym.
	bool isReadyCondition;



	vector<DecisionData> path;

	vector<ConditionGroup> requirements;

	vector<ConditionElement> demandedReqs; //DemandedReqs eto isklyuchitel'no ConditionElement i eto tochno ne ConditionGroup, a napryamuyu sami chasticy trebovanij \
												Tak kak stoit vopros o ConditionGroup Or, demandedReqs popolnitsya TOL'KO po prinyatiyu odnogo iz etih reshenij vo vremya pogruzheniya v novyj mainNode


	MainNode(int parentNode, vector<DecisionData> path, vector<ConditionGroup> requirements, vector<vector<pair<int, int>>> assumption, vector<vector<int>> assumptionsApplied, vector<pair<bool, int>> readyFriendsApplied)
		: parentNode(parentNode), path(path), requirements(requirements)
	{
	}

	MainNode() {}

	MainNode(int parentNode, vector<DecisionData> path, vector<ConditionGroup> requirements)
		: parentNode(parentNode), path(path), requirements(requirements)
	{}

};


class MainNodeGroup {
public:
	vector<MainNode> nodes;

	enum GroupType { And, Or, Xor } groupType;

	MainNodeGroup() {};

	MainNodeGroup(MainNode condition) { //Inicializirovat' ConditionGroup s odnim elementom
		nodes.push_back(condition);
		groupType = And;
	}
	MainNodeGroup(GroupType type) : groupType(type) {}
};


///////////////////////////////// FUNCTIONS

bool IsDemandedElement(MainNode node, ConditionElement* element)
{
	for (int i = 0; i < node.demandedReqs.size(); i++)
	{
		if (node.demandedReqs[i].checkMatchingResultCondition(*element) == 1)
		{
			//Znachit eto ono i est'!
			return true;
		}
	}
	return false;
}

pair<bool, vector<ConditionGroup>> EvaluateDecisionMultiResult(const vector<ConditionGroup>& reqs, const DecisionData& decision, const vector<ConditionGroup>& potentialNewReqs) {

	pair<bool, vector<ConditionGroup>> finalResult = {};


	vector<ConditionGroup> intermediateResult = {};



	for (int r = 0; r < reqs.size(); r++) {

		int countSatisfied = 0; //Dostatochno znat' prosto kolichestvo udovletvoryonnyh podtrebovanij.
		int countWasted = 0;

		ConditionGroup currentGroup = {};
		currentGroup.groupType = reqs[r].groupType;

		for (int rr = 0; rr < reqs[r].conds.size(); rr++) {

			bool addCurrentRequirement = true;
			bool requirementWasted = false;

			//sravnenie s rezul'tatom decision
			for (int res = 0; res < decision.decision->result.size(); res++) {


				Condition* cond = decision.decision->result[res]->makeConditionDynamic(decision.appliedToCourtier);
				int matchingResult = reqs[r].conds[rr]->checkMatchingResultCondition(*cond);

				if (matchingResult == 1) //Eto znachit chto reshenie udovletvorilo dannoe trebovanie!
					addCurrentRequirement = false; //i ne nuzhno ego nikuda dobavlyat', ego bol'she poprostu ne budet
				else if (matchingResult == -1) { //Eto znachit reshenie pogubilo kakoe-to trebovanie! Eto vpolne mozhet byt' konec. No eto mozhet byt' Or

					requirementWasted = true;//on pozzhe proverit, kak svyazana eta shtuka s GroupType
					addCurrentRequirement = false;
				}
			}

			//if(addCurrentRequirement) //Prosto uzhe net neobhodimosti vosprinimat' tekushchee trebovanie kak trebovanie, esli ono udovletvoreno.
			//sravnenie s potencial'nymi novshestvami v trebovaniyah


			/// ///////////////// //////////////////////////////////// AFTER:


			if (!addCurrentRequirement && !requirementWasted) //Esli eto podtrebovanie, kotoroe bylo udovletvoreno

				countSatisfied++;

			else if (addCurrentRequirement)

				currentGroup.conds.push_back(reqs[r].conds[rr]); //Obychnaya situaciya - prosto prinyat' trebovanie k newGroup

			else if (requirementWasted) //Trebovaniyu prtivorechit novaya situaciya, ono nevypolnimo

				countWasted++;

		}

		if (currentGroup.groupType == ConditionGroup::Or)
		{
			if (countSatisfied > 0)		//You don't need to add this conditionGroup already
			{
				//bye-bye!
			}
			else if (currentGroup.conds.size() > 0)		//Udovletvoreniya ne bylo, no dolzhen ostat'sya hot' kakoj-to zapas
			{
				intermediateResult.push_back(currentGroup);
			}
			else	//Eto poslednyaya kaplya. Vsyo
			{
				finalResult.first = false;
				return finalResult;
			}

		}
		else if (currentGroup.groupType == ConditionGroup::And)
		{
			if (countWasted > 0) //i zaputyvat'sya ne nado, eto avtomaticheski konec)
			{
					finalResult.first = false;
					return finalResult;
			}
			else if (currentGroup.conds.size() > 0) //Esli net nikakih poteryannyh, to nichego plohogo uzhe byt' ne mozhet, i dobavit' tol'ko v sluchae esli hot' chto-to iz nih neudovletvoreno
			{
				intermediateResult.push_back(currentGroup);
			}
		}
	}





	for (int nr = 0; nr < potentialNewReqs.size(); nr++) {

		int countSatisfied = 0; //Dostatochno znat' prosto kolichestvo udovletvoryonnyh podtrebovanij.
		int countWasted = 0;

		ConditionGroup currentGroup = {};
		currentGroup.groupType = potentialNewReqs[nr].groupType;

		for (int nrr = 0; nrr < potentialNewReqs[nr].conds.size(); nrr++) {

			bool addCurrentRequirement = true;
			bool requirementWasted = false;


			for (int r = 0; r < intermediateResult.size(); r++) {
				for (int rr = 0; rr < intermediateResult[r].conds.size(); rr++) {


					int matchingResult = potentialNewReqs[nr].conds[nrr]->checkMatchingResultCondition(*intermediateResult[r].conds[rr]);

					if (matchingResult == 1) //Eto znachit chto reshenie udovletvorilo dannoe trebovanie!
						addCurrentRequirement = false; //i ne nuzhno ego nikuda dobavlyat', ego bol'she poprostu ne budet
					else if (matchingResult == -1) { //Eto znachit reshenie pogubilo kakoe-to trebovanie! Mozhet byt' eto konec

						//Logicheski, dostatochno prosto skazat' addCurrentRequirement = false v sluchae s Or, no esli And, to v takom sluchae uzhe konec.

						requirementWasted = true;//on pozzhe proverit, kak svyazana eta shtuka s GroupType
						addCurrentRequirement = false;
					}
				}
			}


			/// ///////////////// //////////////////////////////////// AFTER:


			if (!addCurrentRequirement && !requirementWasted) //Esli eto podtrebovanie, kotoroe bylo udovletvoreno

				countSatisfied++;

			else if (addCurrentRequirement)

				currentGroup.conds.push_back(potentialNewReqs[nr].conds[nrr]); //Zdes' uzhe zasazhivaetsya imenno NewRequirement

			else if (requirementWasted) //Trebovaniyu prtivorechit novaya situaciya, ono nevypolnimo

				countWasted++;

		}

		if (currentGroup.groupType == ConditionGroup::Or)
		{
			if (countSatisfied > 0)		//You don't need to add this conditionGroup already
			{
				//bye-bye!
			}
			else if (currentGroup.conds.size() > 0)		//Udovletvoreniya ne bylo, no dolzhen ostat'sya hot' kakoj-to zapas
			{
				finalResult.second.push_back(currentGroup);
			}
			else	//Eto poslednyaya kaplya. Vsyo
			{
				finalResult.first = false;
				return finalResult;
			}

		}
		else if (currentGroup.groupType == ConditionGroup::And)
		{
			if (countWasted > 0) //i zaputyvat'sya ne nado, eto avtomaticheski konec)
			{
				finalResult.first = false;
				return finalResult;
			}
			else if (currentGroup.conds.size() > 0) //Esli net nikakih poteryannyh, to nichego plohogo uzhe byt' ne mozhet, i dobavit' tol'ko v sluchae esli hot' chto-to iz nih neudovletvoreno
			{
				finalResult.second.push_back(currentGroup);
			}
		}
	}




	for (int i = 0; i < intermediateResult.size(); i++)
	{
		finalResult.second.push_back(intermediateResult[i]);
	}


	finalResult.first = true;
	return finalResult;
}

vector<Decision*> FindTheoreticalDecisionsToSolveRequirement(ConditionElement& requirementToResolve) {

	//mozhet kak raz cherez to derevo s AlgorithmNode..?

	vector<Decision*> foundDecisions = {};

	for (int d = 0; d < decisions.size(); d++) {

		for (int dr = 0; dr < decisions[d]->result.size(); dr++) {

			int res = requirementToResolve.checkMatchingResultCondition(*decisions[d]->result[dr]->makeConditionDynamic(requirementToResolve.B));//requirementToResolve.checkMatchingResultCondition(*res1);

			if (res == 1) //Esli rezul'tat resheniya sopostavlyaetsya s vybrannym trebovaniem, to prinimaem!
				foundDecisions.push_back(decisions[d]);
		}
	}

	return foundDecisions;
}


pair<bool, vector<ConditionGroup>> EvaluateDecisionForRequirement(const Decision& decisionType, const ConditionElement& requirementToResolve, const MainNode& node) {
	//RequirementToResolve eto kastrirovannyj ConditionGroup, ved' ConditionGroup proveryaetsya na bolee vysokom urovne.


	int x = requirementToResolve.B;


		//On tak zhe vozvrashchaet i bool. v tom sluchae esli okazhetsya, chto poyavlyayushchiesya trebovaniya ne sovmestimy s tem, chto proiskhodit v real'nosti,\
		,ved' imenno eta funkciya EvaluateCondition soderzhit v sebe naibol'shee myaso vospriyatiya abstrakcij.
	pair<bool, vector<ConditionGroup>> answer = decisionType.EvaluateCondition.operator()(courtiers, x, node); //Vozvrashchaet naimen'shee (requirements - readyConditions)
	//Imenno Requirements yavlyayutsya formatom ConditionGroup!


	if (answer.first) {

		vector<ConditionGroup> newRequirements = answer.second;



		//Vernut' udovletvoryayushchij otvet.
		return pair<bool, vector<ConditionGroup>>(true, answer.second);
	}
	else {
		//Takoj zhe tupik, s ispol'zovaniem dannogo resheniya, eto trebovanie praktichno vypolnennym okazat'sya ne sposobno
		return pair<bool, vector<ConditionGroup>>(false, NULL);
	}

}

pair<bool, MainNode> FindAlgorithm() { //Vozvrashchaet NULL esli algoritm tak i ne nashyolsya.

	int totalIterations = 0; int demandedIterations = 0; int totalPotential = 0;

	pair<bool, MainNode> FinalAlgorithmResult = {};
	//Oni nastol'ko organizovanno sobirayutsya raspologat'sya, chto dazhe nikakogo razdeleniya glubiny ne trebuetsya
	vector<MainNodeGroup> mainNodes;

	MainNodeGroup stGroup = MainNodeGroup(MainNodeGroup::And);

	ConditionElement element =  ConditionElement(Relationship::Lord, last_court, first_court, 0);

	ConditionGroup group = ConditionGroup(ConditionGroup::And);

	if (courtiers[last_court].relation[first_court] != Relationship::Lord) //////////Zadat' trebovanie algoritmu tol'ko v tom sluchae, esli etogo sostoyaniya poka ne sushchestvuet v udovletvoryonnom sostoyanii
		group.conds.push_back(&element);
	else
		//////My na podvohi ne vedyomsya
		return pair<bool, MainNode>(true, MainNode(-1, vector<DecisionData>(), vector<ConditionGroup>())); //Vse pustye.

	stGroup.nodes.push_back(MainNode(-1, vector<DecisionData>(), vector<ConditionGroup> { group }));




	mainNodes.push_back(stGroup);

	for (int n = 0; n < mainNodes.size(); n++) {

		for (int nn = 0; nn < mainNodes[n].nodes.size(); nn++) { //Elementy gruppy osnovnyh nod. Esli eto ne Or, to eto budet tol'ko odin element i cikl tol'ko na odnu iteraciyu


			bool oneFullRequirementWasted = false;
			bool oneFullRequirementSatisfied = false;



			for (int r = 0; r < mainNodes[n].nodes[nn].requirements.size(); r++) {
				//Vyhodit, v etoj situacii ya beru i prohozhus' po ciklu naoborot, resheniya yavlyayutsya poddejstviem otnositel'no requirements. \
				YAvlyaetsya li eto problemoj privodyashchej k neoptimal'nosti, posmotrim po dal'nejshemu analizu situacii.

				bool oneRequirementResolved = false;
				bool unsatisfiedRR = false;

				vector<vector<MainNode>> newWays; //Etot spisok yavlyaetsya chast'yu kazhdogo pod trebovaniya. \
												  Osnovnoj spisok eto kazhdoe otdel'noe podtrebovanie, a glubnyj spisok eto variacii(raznye resheniya) vypolneniya etogo podtrebovaniya. \
											Kazhdoe odno reshenie odnoznachno podrazumevaet soboj znachenie Or, tak kak kazhdoe reshenie yavlyaetsya kandidatom na reshenie odnogo podtrebovaniya \


				for (int rr = 0; rr < mainNodes[n].nodes[nn].requirements[r].conds.size(); rr++) { //if not 'Or', it will be always size 1. //No, here is And from conspire)

					newWays.push_back(vector<MainNode>()); //Dobavit' novuyu gruppu otvetvlenij

					Condition* basecond = mainNodes[n].nodes[nn].requirements[r].conds[rr];

					//Proverit' tut na to, kakim yavlyaetsya eto trebovanie, i kak ego mozhno pervoprohodno proverit':
					if (auto* cond = dynamic_cast<ConditionElement*>(mainNodes[n].nodes[nn].requirements[r].conds[rr]))//(ti == std::type_index(typeid(ConditionElement)))//(typeid(*basecond).hash_code() == typeid(ConditionElement).hash_code()) //requirement eto ConditionElement
					{
						//Snachala proverit', ne demanded li eto znachenie.
						if (IsDemandedElement(mainNodes[n].nodes[nn], cond))
						{//bye-bye! Vot eto nastoyashchaya tochka UnsatisfiedRR
							unsatisfiedRR = true; demandedIterations++; continue; //Prodolzhit' sleduyushchij rr
						}


						vector<Decision*> potentialDecisions = FindTheoreticalDecisionsToSolveRequirement(*cond);//mainNodes[n].nodes[nn].requirements[r].conds[rr]);
						int lostDecisions = 0;
						totalPotential += potentialDecisions.size();

						for (int d = 0; d < potentialDecisions.size(); d++) {


							pair<bool, vector<ConditionGroup>> result = EvaluateDecisionForRequirement(*potentialDecisions[d], *cond, mainNodes[n].nodes[nn]);
							//Esli EvaluateDecisionForRequirement vozvrashchaet true no ne vozvrashchaet trebovaniya, znachit eto ReadyCondition, i on mozhet pustit' signal zavershyonnosti dannogo resheniya

							if (!result.first) {
								lostDecisions++;
								//unsatisfiedRR = true; //Polezno v tom sluchae, kogda zadan And s neskol'kimi trebovaniyami odnovremenno. Esli hot' odno ne sposobno byt' vypolneno, eto odnoznachno sudnyj den' dlya vetki.
								continue; //continue iterating through different decisions
							}
							else {
								//Potencial'noe reshenie potencial'no udovletvoreno!

								//Esli trebovaniyu nashyolsya otvet, to udalyaetsya ves' ConditionGroup celikom, dazhe proverki na Or ne nuzhno)
								//No pervoe udovletvorivshee dejstvie ne znachit edinstvennoe, poetomu ne stoit toropit'sya, nuzhno skopit' vse srabotavshie dejstviya.

								DecisionData decisionData(potentialDecisions[d], cond->B);

								//Tak, nuzhno proveryat' naskol'ko dannoe reshenie reshaet ostal'nye trebovaniya!
								//Itak, esli vybrannoe reshenie mozhet protivorechit' drugomu trebovaniyu, to vot vpolne reshenie - vozvrashchenie ne tol'ko udovletvoryonnyh trebovanij,\
								,no i naoborot, povrezhdenie trebovanij.
								//Ono vozvrashchaet prosto bool i vector<ConditionGroup>. Esli hot' odno trebovanie postradalo, to eto odnoznachnyj konec i break etoj vetvi.
								pair<bool, vector<ConditionGroup>> updatedRequirements = EvaluateDecisionMultiResult(mainNodes[n].nodes[nn].requirements, decisionData, result.second); //passing by reference
								//Imenno eta funkciya pridast prioritet etim vetvyam dlya nahozhdeniya optimal'nosti.

								if (updatedRequirements.first == false) { //Eshchyo odna proverka, kotoraya mozhet reshit', chto dannoe reshenie ne podhodit

									//unsatisfiedRR = true;
									lostDecisions++; //Eto tozhe, v tom chisle prosto poteryannoe potencial'noe reshenie
									continue; //continue potential decisions
								}
								else
								{
									//Eta vetka oznachaet, chto reshenie proshlo vse proverki i budet uchtyono dlya osnovnogo dreva.
									oneRequirementResolved = true;
									totalIterations++;


									vector<DecisionData> data = mainNodes[n].nodes[nn].path; //Copy

									//Etot parametr x eshchyo ne yasen, naskol'ko on budet dejstvitel'no otobrazhat' aktual'noe znachenie x.
									data.push_back(decisionData);

									MainNode newNode = MainNode(n, data, updatedRequirements.second);



									/////////////////DOES NOT EXIST FOR K = N - 1
																	//Novye trebovaniya, chtoby uznat', ne sdelat' li assumption.last_court
									//UpdateAssumptionAndValueConditions(&newNode, result.second, decisionData, *cond); //chtoby on sdelal vsyo delo vnutri sebya. Eta funkciya tak zhe obnovit znacheniya Fs.

									//Imenno konkretno vybrannyj decision dobavit znachenie demanded, kotoroe eto reshenie pytaetsya udovletvorit'
									newNode.demandedReqs = mainNodes[n].nodes[nn].demandedReqs;
									newNode.demandedReqs.push_back(*cond);


									newWays[newWays.size() - 1].push_back(newNode); //Posle polnoj gotovnosti, vsunut' ego v obshchij spisok newWays



									if (newNode.requirements.size() == 0) //Konechnaya tochka. Esli novoe zveno eto zveno, ne soderzhashchee ni edinogo trebovaniya, znachit eto chistejshaya POBEDA.
									{
										FinalAlgorithmResult.first = true;
										FinalAlgorithmResult.second = newNode;
										return FinalAlgorithmResult; //FINALE
									}
								}




							}
						}

						if (lostDecisions == potentialDecisions.size())
							unsatisfiedRR = true; //Esli ne sluchilos' ni edinogo resheniya, kotoroe by podhodilo, znachit kaput RR
					}

				}



				//////////////////////////////////////////////////////////////////RESULT ADDING/////////////////////////////////////////////////////////////////////

				//Eta proverka na Or i And dolzhna byt' proverkoj posle vsej etoj figni, a ne vnutri neyo
				//Tak zhe zdes' opredelitsya to, kak newWays raspredelyatsya po MainNodeGroup
				//Tak zhe v etom meste budet ustanavlivat'sya to, kak raspredelitsya i zakrepityas ierarhiya mezhdu predydushchimi nodami

				if (mainNodes[n].nodes[nn].requirements[r].groupType == ConditionGroup::Or) {
					//Esli eto Or, to vypolnenie dolzhno prodolzhat'sya v raznyh osnovnyh vetkah pri vybore variativnosti


					if (oneRequirementResolved) {
						//Eto znachit vsyo horosho, hot' chto-to srabotalo i etogo dostatochno.
						oneFullRequirementSatisfied = true;

						//MainNodeGroup ne dolzhna zaviset' ot predydushchih dannyh, ona srozdayotsya zdes', idya dal'she v glubinu! Pryamo iz newWays
						MainNodeGroup newGroup = MainNodeGroup();
						newGroup.groupType = MainNodeGroup::Or;
						//Esli eto Or, to prosto vse elementy zasypayutsya v odnu gruppu MainNodeGroup-Or.

						for (int i = 0; i < newWays.size(); i++) for (int j = 0; j < newWays[i].size(); j++)

							newGroup.nodes.push_back(newWays[i][j]);

						mainNodes.push_back(newGroup); //Novoe osnovnoe zveno gotovo!

					}
					else {
						//Hot' eto i Or, no ni edinogo resheniya dlya dannogo requirement ne nashlos'. Eto avtomaticheski znachit chto newWays pust.
						//Znachit, dannaya vetka pogibla

						//Poka chto mozhno prenebrech' otrubaniem vetki i ispol'zovaniem variationRequirementCreationMainNode. Eto chistoe zanyatie dlya proizvoditel'nosti.
						//mainNodes[n].nodes[nn].requirements[r].variationRequirementCreationMainNode

						//No kak minimum, ne proizojdyot vychisleniya sleduyushchego trebovaniya dlya dannogo zvena.
						oneFullRequirementWasted = true;
						break; //Pust' on vyjdet v osnovnoj cikl (nn), chtoby tam on razobralsya, cherez chto on budet pereskakivat'.
					}

				}
				else if (mainNodes[n].nodes[nn].requirements[r].groupType == ConditionGroup::And) {

					//Esli eto And, to trebovaniya dolzhny byt' vmeste v ramkah lyuboj vetki. And ne yavlyaetsya variativnost'yu.

					if (unsatisfiedRR) {

						oneFullRequirementWasted = true;
						break;
					}
					else { //Esli net ne vypolnennyh, znachit vypolneny vse.

						//Tak kak eto AND, to newWays raspredelyaetsya kak AND ( OR, OR, OR... ), a eto polnost'yu ekvivalentno tomu, \
									chtoby byla dobavlena prosto posledovatel'nost' MainNodeGroup, kotorye po otdel'nosti budut yavlyat'sya Or

						oneFullRequirementSatisfied = true;

						for (int i = 0; i < newWays.size(); i++)
						{
							MainNodeGroup newGroup = MainNodeGroup();
							newGroup.groupType = MainNodeGroup::Or;

							for (int j = 0; j < newWays[i].size(); j++)
								newGroup.nodes.push_back(newWays[i][j]);


							if (newWays[i].size() != 0) //Tak kak po kazhdomu rr dobavlyaetsya po novomu newWay, on mozhet tak i ne zapolnitsya.
								mainNodes.push_back(newGroup); //Novoe osnovnoe zveno gotovo!
						}
					}

				}

			}


			if (oneFullRequirementWasted)
			{
				//ispol'zovanie goto zdes' privodit k gorazdo bol'shej intuitivnosti proiskhodyashchego

				if (mainNodes[n].groupType == MainNodeGroup::And)
					goto BackToNextMainNode; //srazu otkat na sleduyushchij n
				else if (mainNodes[n].groupType == MainNodeGroup::Or)
					goto BackToNextSubNode; //prosto prodolzhit' sleduyushchij nn
			}

		BackToNextSubNode:;
		}
	BackToNextMainNode:;
		//Kak bonus, mozhno udalit' tekushchij n, ved' programma k nemu nikogda bol'she ne vernyotsya.
		mainNodes.erase(mainNodes.begin());
		n--; //ahah, poluchaetsya n prosto konstantnyj, raven nolyu.
	}

	/// /// Proshyol vse vetvi, no vyhoda tak i ne nashyol

	FinalAlgorithmResult.first = false;
	return FinalAlgorithmResult;
}



/////////////////////////////////////




void DefineDecisions() {


	//ConditionElement* element1 = ;
	Decision* MakeFriend2 = new Decision(
		"MakeFriend2", //-1=first_court. -2=last_court. -3=x. -4=y. -5=z. -6=w
							//Tut osobo ne stoi volnovat'sya po povodu ValueAspiration
		vector<Condition*> { new ConditionElement(Relationship::Friend, -2, -3, -1) }, //dlya static, stPoint eto tak zhe chto-to abstraktnoe, -1
		[](const vector<Courtier>& data, const int& x, const MainNode& node) -> pair<bool, vector<ConditionGroup>> {

			pair<bool, vector<ConditionGroup>> result = {};
			//pair<vector<Condition>, vector<ConditionGroup>> reqs = {};


			ConditionGroup variation(ConditionGroup::Or);

			bool necessaryComplete = false;
			bool foundIdeal = false; //Esli dlya etogo resheniya najdena ideal'naya kombinaciya, to nikakih variacij requirements ne trebuetsya\
								foundIdeal eto mozhno skazat' ReadyCondition

			for (int i = 0; i < N - 1; i++) if (i != x) {

				if (data[x].relation[i] == Relationship::Friend) { //necessary
					necessaryComplete = true;

					if (data[i].relation[last_court] == Relationship::Friend) //kogda vsyo ideal'no, on byl po umolchaniyu drugom, i na nego ne bylo nalozheno QuarrelReadyFriends.
					{
						foundIdeal = true;
					}
					else
						variation.conds.push_back(new ConditionElement(Relationship::Friend, last_court, i, node.path.size())); //Ochevidno zdes' x eto i.

				}
			}

			if (!necessaryComplete) { result.first = false; return result; } //Esli neobhodimoe trebovanie ne vypolneno, to eto pipec.


			if (!foundIdeal)//(variation.conds.size() > 0 and !foundIdeal) Esli proshlo necessaryComplete i v tozhe vremya !foundIdeal, to conds.size v lyubom sluchae budet bol'she 0.
				result.second.push_back(variation);



			result.first = true;
			return result;
		}
	);


	//ConditionElement element2 = ;
	Decision* MakeFriend1 = new Decision(
		"MakeFriend1", //-1=first_court. -2=last_court. -3=x. -4=y. -5=z. -6=w
							//Tut osobo ne stoi volnovat'sya po povodu ValueAspiration
		vector<Condition*> { new ConditionElement(Relationship::Friend, -2, -3, -1) },
		[](const vector<Courtier>& data, const int& x, const MainNode& node) -> pair<bool, vector<ConditionGroup>> {

			pair<bool, vector<ConditionGroup>> result = {};

			bool necessaryComplete = false;

			for (int i = 0; i < N; i++) if (i != x) {

				//V pravilah ne ukazano, chto x ne mozhet byt' slugoj imenno last_court. No on ne mozhet byt' slugoj samogo sebya, proverka if prosto dlya osoznaniya etogo utochneniya
				if (data[x].relation[i] == Relationship::Servant)
					necessaryComplete = true;
			}

			if (!necessaryComplete) { result.first = false; return result; }


			bool isServant = false; //Zdes' isServant yavlyaetsya analogom foundIdeal

			ConditionGroup variation(ConditionGroup::Or);
			variation.conds.reserve(N - 1);

			for (int i = 0; i < N - 1; i++) {

				//last_court dolzhen byt' ch'im-to slugoj
				//Tut variaciya eto ves' spektr ot 0 do N-1)

				if (data[last_court].relation[i] == Relationship::Servant)
					isServant = true;

				variation.conds.push_back(new ConditionElement(Relationship::Servant, last_court, i, node.path.size())); //Vse varianty kotorye mogut byt' u intrigana, chtoby stat' slugoj
			}

			if (!isServant)
				result.second.push_back(variation);

			result.first = true;
			return result;
		}
	);

	//ConditionElement element3 = ;
	Decision* MakeServant = new Decision(
		"MakeServant", //-1=first_court. -2=last_court. -3=x. -4=y. -5=z. -6=w
							//Tut osobo ne stoi volnovat'sya po povodu ValueAspiration
		vector<Condition*> { new ConditionElement(Relationship::Lord, -2, -3, -1) },
		[](const vector<Courtier>& data, const int& x, const MainNode& node) -> pair<bool, vector<ConditionGroup>> {

			pair<bool, vector<ConditionGroup>> result = {};

			ConditionGroup variation(ConditionGroup::Or);

			bool necessaryComplete = false;
			bool foundIdeal = false;


			for (int i = 0; i < N - 1; i++) if (i != x) {
				//Zdes' i eto ta samaya z

				if (data[i].relation[x] == Relationship::Friend) {//necessary

					necessaryComplete = true;

					if (data[last_court].relation[i] == Relationship::Lord)
						foundIdeal = true;
					else
						variation.conds.push_back(new ConditionElement(Relationship::Lord, last_court, i, node.path.size()));
				}
			}

			if (!necessaryComplete) { result.first = false; return result; }


			if (!foundIdeal) result.second.push_back(variation);


			result.first = true;
			return result;
		}
	);

	//ConditionElement element4 = ;
	Decision* Conspire = new Decision(
		"Conspire", //-1=first_court. -2=last_court. -3=x. -4=y. -5=z. -6=w
							//Tut osobo ne stoi volnovat'sya po povodu ValueAspiration
		vector<Condition*> { new ConditionElement(Relationship::Lord, -2, -3, -1) },
		[](const vector<Courtier>& data, const int& x, const MainNode& node) -> pair<bool, vector<ConditionGroup>> {

			pair<bool, vector<ConditionGroup>> result = {};

			ConditionGroup variation(ConditionGroup::And); //Tut variaciya odnoznachno And. Libo, esli budut s etim problemy, prosto budu delat' novyj ConditionGroup dlya etogo.

			bool necessaryComplete = false;
			bool foundIdeal = true;


			for (int i = 0; i < N - 1; i++) if (i != x) {
				//Zdes' i eto ta samaya z


				if (data[x].relation[i] == Relationship::Friend) {
					//Esli u nego est' drug, to on obyazan byt' i drugom intrigana

					necessaryComplete = true; //Esli u x est' hot' odin drug, to eto uzhe odnoznachno oznachaet, chto on potencial'no mozhet stat' drugom intrigana, eto edinstvennoe obyazatel'stvo.

					if (data[last_court].relation[i] != Relationship::Friend) {
						foundIdeal = false;
						variation.conds.push_back(new ConditionElement(Relationship::Friend, last_court, i, node.path.size())); //vo kak
					}
				}

			}


			if (!necessaryComplete) { result.first = false; return result; }
			else if (!foundIdeal)
				result.second.push_back(variation);


			result.first = true;
			return result;
		}
	);

	//ConditionElement element5 = ;
	Decision* BecomeServant = new Decision(
		"BecomeServant", //-1=first_court. -2=last_court. -3=x. -4=y. -5=z. -6=w
							//Tut vezde dolzhno budet byt' -2, -3)
		vector<Condition*> { new ConditionElement(Relationship::Servant, -2, -3, -1) },
		[](const vector<Courtier>& data, const int& x, const MainNode& node) -> pair<bool, vector<ConditionGroup>> {

			pair<bool, vector<ConditionGroup>> result = {};


			bool necessaryComplete = true;

			//Vyhodit, esli skhitrit', poluchaetsya chto dejstvie Quarrel budet vypolneno tol'ko v nachale, ved' pozzhe net smyla umen'shat' sebe druzej. \
			Ne pohozhe chtoby algoritm s takim obratnym podhodom mog by tak chyotko prorabayvat' hronologicheskie dejstviya "snachala podruzhit'sya, sdelat' dejstvie i ubrat' druzhbu"

			for (int i = 0; i < N - 1; i++) {
				if (data[last_court].relation[i] == Relationship::Servant)
					necessaryComplete = false;
			}



			if (!necessaryComplete) { result.first = false; return result; }
			else
			{
				result.first = true;
				return result;
			}
		}
	);

	decisions = { MakeFriend1, MakeFriend2, MakeServant, Conspire, BecomeServant };
}



///////////////////////////////////////////// TEST!!!!

void Iterate(int relativeToCourtier,
	std::function<bool(const int& index, const Relationship& relation)> rule) {

	for (int i = 0; i < N; i++) {

		if (rule(i, courtiers[relativeToCourtier].relation[i]))
			return;
	}
}

/////////Tak zhe vozmozhno i tak napisat' esli trebuetsya
void MakeRelation_Friendship(int A, int B) { //Friendship is magic!
	courtiers[A].relation[B] = Relationship::Friend;
	courtiers[B].relation[A] = Relationship::Friend;
}
void MakeRelation_Ancillary(int A, int B) { //A - Lord, B - Servant
	courtiers[A].relation[B] = Relationship::Lord;
	courtiers[B].relation[A] = Relationship::Servant;
}
void RemoveRelation(int A, int B) {
	courtiers[A].relation[B] = Relationship::None;
	courtiers[B].relation[A] = Relationship::None;
}

void MakeFriend(int operand) {   /////////////////MakeFriend x

	int friendsCount = 0;
	Iterate(last_court, [&](const int& i, const Relationship& r) -> bool {
		if (r == Relationship::Friend) friendsCount++;
		return false;
		});

	if (friendsCount == K) throw "Friends number cannot exceed K";

	bool yes;

	Iterate(last_court, [&](auto i, auto r) -> bool {

		if (r == Relationship::Friend and courtiers[i].relation[operand] == Relationship::Friend)
		{
			yes = true;
			return true; //finish iterating
		}
		else return false; //continue iterating
		});

	if (yes)
		MakeRelation_Friendship(last_court, operand);
	else throw "Rules refused to make friend with first method";
}


void MakeFriend2(int operand) {    ////////////////MakeFriend x (second condition)

	int friendsCount = 0;
	Iterate(last_court, [&](auto i, auto r) -> bool {
		if (r == Relationship::Friend) friendsCount++;
		return false;
		});

	if (friendsCount == K) throw "Friends number cannot exceed K";

	bool yes = false;

	bool isServant = false;
	Iterate(last_court, [&](auto i, auto r) -> bool {
		if (r == Relationship::Servant) isServant = true;
		return false;
		});

	if (isServant)
		Iterate(operand, [&](auto i, auto r) -> bool {

		if (r == Relationship::Servant)
		{
			yes = true;
			return true; //finish iterating
		}
		return false; //continue iterating
			});

	if (yes)
		MakeRelation_Friendship(last_court, operand);
	else throw "Rules refused to make friend with second method";
}


void MakeServant(int operand) {

	bool yes = false;

	Iterate(last_court, [&](auto i, auto r) -> bool {

		if (r == Relationship::Lord and courtiers[i].relation[operand] == Relationship::Friend)
		{
			yes = true;
			return true; //finish iterating
		}
		else return false; //continue iterating
		});

	if (yes)
		MakeRelation_Ancillary(last_court, operand);
	else throw "Rules refused to make servant";
}

void BecomeServant(int operand) {

	bool isServant = false;
	Iterate(last_court, [&](auto i, auto r) -> bool {
		if (r == Relationship::Servant) isServant = true;
		return false;
		});

	if (isServant) throw "Intriguant is not able to become a servant twice";

	MakeRelation_Ancillary(operand, last_court);
}

void Conspire(int operand) {

	bool yes = true;

	Iterate(operand, [&](auto i, auto r) -> bool {

		if (r == Relationship::Friend and (i != operand) and (i != last_court)) //i eto drug operanda
		{
			if (courtiers[i].relation[last_court] != Relationship::Friend) //YAvlyaetsya drugom operanda, no ne drugom intrigana!
			{

				yes = false;
				return true; //finish
			}
			else return false;//Do not finish! nuzhno proverit' vsekh druzej ot nachala do konca
		}
		else return false; //continue iterating
		});

	if (yes)
		MakeRelation_Ancillary(last_court, operand);
	else throw "Rules refused to conspire";
}

void Quarrel(int operand)
{
	if (courtiers[last_court].relation[operand] == Relationship::Friend)
		RemoveRelation(last_court, operand);
	else
		throw "Cannot Quarrel when it's not even a friend";
}




void InitializeCourtiers()
{
	courtiers = vector<Courtier>(N, Courtier(N));
}


int main() {

	ActionType type = ActionType::MakeFriend;

	Relationship relation = Relationship::Friend;



	DefineDecisions();



	//SINTAKS - probely obazatel'ny.

	ifstream file("plots.in");

	string word;


	int iteration = 0;

	///////////////Vsyo eto schityvanie fajla plots.in
	if (file.is_open())
	{
		getline(file, word, ' '); //Dostat' pervoe chislo.
		N = stoi(word);
		last_court = N - 1;
		first_court = 0;
		cout << word << endl;

		getline(file, word); //Dostat' vtoroe chislo.
		K = N - 1;////stoi(word); // K = N - 1!!
		cout << word << endl;
		cout << "Word received " << K;

		InitializeCourtiers();

		string relation;
		int courtier1 = -1;
		int courtier2 = -1;

		cout << "starting loop!" << endl;

		char separator = ' ';

		while (getline(file, word, separator))
		{

			switch (iteration)
			{
			case 0:
				relation = word;
				iteration = 1; separator = ' ';
				break;
			case 1:
				courtier1 = stoi(word) - 1; //Strukturu (1 - N) nuzhno perevesti v (0 - N-1)
				iteration = 2; separator = '\n';
				break;
			case 2:
				courtier2 = stoi(word) - 1;

				if (relation == ">")
				{
					courtiers[courtier1].relation[courtier2] = Relationship::Lord;
					courtiers[courtier2].relation[courtier1] = Relationship::Servant;
				}
				else if (relation == "=")
				{
					courtiers[courtier1].relation[courtier2] = Relationship::Friend;
					courtiers[courtier2].relation[courtier1] = Relationship::Friend;
				}

				iteration = 0; //Otkatit' do nolya. Ispol'zovanie zdes' ostatka deleniya eto ne samoe effektivnoe reshenie.
				separator = ' ';
				break;
			}

			cout << word << endl;
		}
		file.close();
	}
	else
		cout << "file wasn't found\n";


	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			cout << (int)courtiers[i].relation[j];
		}
		cout << endl;
	}

	cout << endl << "Nachinaetsya FindAlgorithm!" << endl << endl;

	pair<bool, MainNode> finalNode = FindAlgorithm();

	if (finalNode.first == false)
	{
		cout << "The answer is NOT" << endl;
		ofstream fout;
		fout.open("plots.out");
		fout << "NOT" << endl;
		fout.close();
	}
	else
	{
		ofstream fout;
		fout.open("plots.out");
		for (int i = finalNode.second.path.size() - 1; i >= 0; i--)
		{
				cout << finalNode.second.path[i].decision->name << "  " << (finalNode.second.path[i].appliedToCourtier + 1) << endl;
				//fout << finalNode.second.path[i].decision->name << "  " << (finalNode.second.path[i].appliedToCourtier + 1) << endl;

				if (finalNode.second.path[i].decision->name == "MakeFriend1" || finalNode.second.path[i].decision->name == "MakeFriend2")
						fout << "=  " << (finalNode.second.path[i].appliedToCourtier + 1) << endl;
				if (finalNode.second.path[i].decision->name == "Quarell")
						fout << "x  " << (finalNode.second.path[i].appliedToCourtier + 1) << endl;
				if (finalNode.second.path[i].decision->name == "Conspire")
						fout << "#  " << (finalNode.second.path[i].appliedToCourtier + 1) << endl;
				if (finalNode.second.path[i].decision->name == "MakeServant")
						fout << ">  " << (finalNode.second.path[i].appliedToCourtier + 1) << endl;
				if (finalNode.second.path[i].decision->name == "BecomeServant")
						fout << "s  " << (finalNode.second.path[i].appliedToCourtier + 1) << endl;

		}
		fout.close();


		/////////////NOW, LET'S TEST IT

		for (int i = finalNode.second.path.size() - 1; i >= 0; i--)
		{
			string name = finalNode.second.path[i].decision->name;
			int courtier = finalNode.second.path[i].appliedToCourtier;

			if (name == "BecomeServant")
				BecomeServant(courtier);
			else if (name == "MakeFriend1")
				MakeFriend2(courtier);
			else if (name == "MakeFriend2")
				MakeFriend(courtier);
			else if ((name == "Quarrel"))
				Quarrel(courtier);
			else if (name == "MakeServant")
				MakeServant(courtier);
			else if (name == "Conspire")
				Conspire(courtier);
		}

		if (courtiers[last_court].relation[first_court] == Relationship::Lord)
			cout << endl << " -----  VICTORY  -----" << endl;
		else
			cout << endl << " ----- SMTH WENT WRONG...  -----" << endl;
	}

	return 0;
}
