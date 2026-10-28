#include <iostream>
#include <memory>
#include "Match.hpp"
#include "T20MatchType.hpp"
#include "Team/Team.hpp"
#include "Team/PlayerDetails.hpp"

using namespace Cricbuzz;

// Helper function to create a player
std::shared_ptr<Player> createPlayer(const std::string& id, const std::string& name,
                                      int age, const std::string& country,
                                      PlayerType type, const std::string& jersey) {
    PlayerDetails details(jersey, "Right-handed", "");
    return std::make_shared<Player>(id, name, age, country, type, details);
}

int main() {
    std::cout << "╔═══════════════════════════════════════════════════════════╗\n";
    std::cout << "║              CRICBUZZ - CRICKET SCORING SYSTEM             ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════════╝\n\n";

    // Create Team India
    auto india = std::make_shared<Team>("IND", "India", "India");
    india->addPlayer(createPlayer("P1", "Rohit Sharma", 36, "India", PlayerType::BATSMAN, "45"));
    india->addPlayer(createPlayer("P2", "Shubman Gill", 24, "India", PlayerType::BATSMAN, "77"));
    india->addPlayer(createPlayer("P3", "Virat Kohli", 35, "India", PlayerType::BATSMAN, "18"));
    india->addPlayer(createPlayer("P4", "KL Rahul", 31, "India", PlayerType::WICKET_KEEPER, "1"));
    india->addPlayer(createPlayer("P5", "Hardik Pandya", 30, "India", PlayerType::ALL_ROUNDER, "33"));
    india->addPlayer(createPlayer("P6", "Ravindra Jadeja", 35, "India", PlayerType::ALL_ROUNDER, "8"));
    india->addPlayer(createPlayer("P7", "Axar Patel", 30, "India", PlayerType::ALL_ROUNDER, "20"));
    india->addPlayer(createPlayer("P8", "Jasprit Bumrah", 30, "India", PlayerType::BOWLER, "93"));
    india->addPlayer(createPlayer("P9", "Mohammed Siraj", 29, "India", PlayerType::BOWLER, "13"));
    india->addPlayer(createPlayer("P10", "Kuldeep Yadav", 29, "India", PlayerType::BOWLER, "23"));
    india->addPlayer(createPlayer("P11", "Mohammed Shami", 33, "India", PlayerType::BOWLER, "11"));
    india->setCaptain(india->getPlayer(0));  // Rohit Sharma
    india->setWicketKeeper(india->getPlayer(3));  // KL Rahul

    // Create Team Australia
    auto australia = std::make_shared<Team>("AUS", "Australia", "Australia");
    australia->addPlayer(createPlayer("A1", "David Warner", 37, "Australia", PlayerType::BATSMAN, "31"));
    australia->addPlayer(createPlayer("A2", "Travis Head", 30, "Australia", PlayerType::BATSMAN, "5"));
    australia->addPlayer(createPlayer("A3", "Steve Smith", 35, "Australia", PlayerType::BATSMAN, "49"));
    australia->addPlayer(createPlayer("A4", "Glenn Maxwell", 35, "Australia", PlayerType::ALL_ROUNDER, "32"));
    australia->addPlayer(createPlayer("A5", "Marcus Stoinis", 34, "Australia", PlayerType::ALL_ROUNDER, "99"));
    australia->addPlayer(createPlayer("A6", "Alex Carey", 32, "Australia", PlayerType::WICKET_KEEPER, "27"));
    australia->addPlayer(createPlayer("A7", "Mitchell Marsh", 32, "Australia", PlayerType::ALL_ROUNDER, "8"));
    australia->addPlayer(createPlayer("A8", "Pat Cummins", 30, "Australia", PlayerType::BOWLER, "30"));
    australia->addPlayer(createPlayer("A9", "Mitchell Starc", 34, "Australia", PlayerType::BOWLER, "56"));
    australia->addPlayer(createPlayer("A10", "Josh Hazlewood", 33, "Australia", PlayerType::BOWLER, "38"));
    australia->addPlayer(createPlayer("A11", "Adam Zampa", 31, "Australia", PlayerType::BOWLER, "63"));
    australia->setCaptain(australia->getPlayer(7));  // Pat Cummins
    australia->setWicketKeeper(australia->getPlayer(5));  // Alex Carey

    std::cout << india->toString() << "\n";
    std::cout << australia->toString() << "\n";

    // Create T20 Match
    auto matchType = std::make_unique<T20MatchType>();
    matchType->setupMatch();
    
    std::cout << "\n" << matchType->getDescription() << "\n\n";

    auto match = std::make_unique<Match>(
        "IND-AUS-T20-001", india, australia, std::move(matchType),
        "Melbourne Cricket Ground, Australia");

    std::cout << match->toString() << "\n";

    // Conduct Toss
    match->conductToss(india, "bat");

    // Start First Innings - India Batting
    match->startInning(india, australia);

    // Get bowler
    auto starc = australia->getPlayerById("A9");
    match->startOver(starc);

    // Simulate some balls
    std::cout << ">>> Simulating first over...\n\n";
    
    match->playBall(BallType::NORMAL, 0, RunType::ZERO);  // Dot ball
    match->playBall(BallType::NORMAL, 4, RunType::FOUR);  // Four!
    match->playBall(BallType::WIDE, 1, RunType::WIDE);    // Wide
    match->playBall(BallType::NORMAL, 1, RunType::ONE);   // Single
    match->playBall(BallType::NORMAL, 6, RunType::SIX);   // Six!
    match->playBall(BallType::NORMAL, 2, RunType::TWO);   // Two runs
    match->playBall(BallType::NORMAL, 0, RunType::ZERO);  // Dot ball

    // Start second over with different bowler
    auto hazlewood = australia->getPlayerById("A10");
    match->startOver(hazlewood);

    std::cout << ">>> Simulating second over...\n\n";
    
    match->playBall(BallType::NORMAL, 1, RunType::ONE);
    match->playBall(BallType::NORMAL, 0, RunType::ZERO);
    
    // Wicket! Bowled
    auto striker = match->getCurrentInning()->getStriker();
    auto wicket = std::make_shared<Wicket>(
        WicketType::BOWLED, striker, hazlewood,
        match->getCurrentInning()->getCurrentOverNumber(),
        match->getCurrentInning()->getBallsInCurrentOver(),
        match->getCurrentInning()->getTotalRuns());
    match->playBall(BallType::NORMAL, 0, RunType::ZERO, wicket);

    match->playBall(BallType::NORMAL, 4, RunType::FOUR);
    match->playBall(BallType::NO_BALL, 2, RunType::NO_BALL);  // No ball + 1 run
    match->playBall(BallType::FREE_HIT, 6, RunType::SIX);     // Free hit six!
    match->playBall(BallType::NORMAL, 1, RunType::ONE);

    // End innings and show scorecard
    match->endInning();

    std::cout << "\n=== INDIVIDUAL PLAYER STATS ===\n\n";
    
    // Show batting stats for opening batsmen
    auto rohit = india->getPlayer(0);
    std::cout << rohit->getScoreSummary() << "\n\n";

    // Show bowling stats
    std::cout << starc->getName() << " Bowling: " 
              << starc->getBowlingScoreCard()->toString() << "\n";
    std::cout << hazlewood->getName() << " Bowling: " 
              << hazlewood->getBowlingScoreCard()->toString() << "\n";

    std::cout << "\n╔═══════════════════════════════════════════════════════════╗\n";
    std::cout << "║              SIMULATION COMPLETE                           ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════════╝\n";

    return 0;
}
