#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "MatchType.hpp"
#include "T20MatchType.hpp"
#include "OneDayMatchType.hpp"
#include "Team/Team.hpp"
#include "Inning/InningDetails.hpp"
#include "ScoreUpdater/ScoreUpdaterObserver.hpp"
#include "ScoreUpdater/BattingScoreUpdater.hpp"
#include "ScoreUpdater/BowlingScoreUpdater.hpp"

namespace Cricbuzz {

/**
 * @brief Enum for match result
 */
enum class MatchResult {
    IN_PROGRESS,
    TEAM1_WON,
    TEAM2_WON,
    TIE,
    DRAW,
    NO_RESULT,
    ABANDONED
};

inline std::string matchResultToString(MatchResult result) {
    switch (result) {
        case MatchResult::IN_PROGRESS: return "In Progress";
        case MatchResult::TEAM1_WON: return "Team 1 Won";
        case MatchResult::TEAM2_WON: return "Team 2 Won";
        case MatchResult::TIE: return "Match Tied";
        case MatchResult::DRAW: return "Match Drawn";
        case MatchResult::NO_RESULT: return "No Result";
        case MatchResult::ABANDONED: return "Abandoned";
        default: return "Unknown";
    }
}

/**
 * @brief Class representing a cricket match
 */
class Match {
private:
    std::string matchId_;
    std::shared_ptr<Team> team1_;
    std::shared_ptr<Team> team2_;
    std::unique_ptr<MatchType> matchType_;
    std::string venue_;
    std::string matchDate_;
    
    std::vector<std::shared_ptr<InningDetails>> innings_;
    std::shared_ptr<InningDetails> currentInning_;
    int currentInningNumber_ = 0;
    
    std::shared_ptr<Team> tossWinner_;
    std::string tossDecision_;  // "bat" or "bowl"
    MatchResult result_ = MatchResult::IN_PROGRESS;
    std::shared_ptr<Team> winner_;
    
    // Observer pattern for score updates
    std::vector<std::shared_ptr<ScoreUpdaterObserver>> scoreObservers_;

    std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%Y-%m-%d");
        return ss.str();
    }

public:
    Match(const std::string& matchId, std::shared_ptr<Team> team1,
          std::shared_ptr<Team> team2, std::unique_ptr<MatchType> matchType,
          const std::string& venue)
        : matchId_(matchId)
        , team1_(team1)
        , team2_(team2)
        , matchType_(std::move(matchType))
        , venue_(venue)
        , matchDate_(getCurrentTimestamp()) {
        
        // Register default observers
        registerObserver(std::make_shared<BattingScoreUpdater>());
        registerObserver(std::make_shared<BowlingScoreUpdater>());
    }

    // Getters
    std::string getMatchId() const { return matchId_; }
    std::shared_ptr<Team> getTeam1() const { return team1_; }
    std::shared_ptr<Team> getTeam2() const { return team2_; }
    MatchType* getMatchType() const { return matchType_.get(); }
    std::string getVenue() const { return venue_; }
    MatchResult getResult() const { return result_; }
    std::shared_ptr<Team> getWinner() const { return winner_; }
    std::shared_ptr<InningDetails> getCurrentInning() const { return currentInning_; }

    /**
     * @brief Register score update observer
     */
    void registerObserver(std::shared_ptr<ScoreUpdaterObserver> observer) {
        scoreObservers_.push_back(observer);
    }

    /**
     * @brief Remove observer
     */
    void removeObserver(std::shared_ptr<ScoreUpdaterObserver> observer) {
        scoreObservers_.erase(
            std::remove(scoreObservers_.begin(), scoreObservers_.end(), observer),
            scoreObservers_.end());
    }

    /**
     * @brief Notify all observers of score update
     */
    void notifyObservers(std::shared_ptr<BallDetails> ballDetails) {
        for (auto& observer : scoreObservers_) {
            observer->update(ballDetails);
        }
    }

    /**
     * @brief Conduct toss
     */
    void conductToss(std::shared_ptr<Team> winner, const std::string& decision) {
        tossWinner_ = winner;
        tossDecision_ = decision;
        
        std::cout << "\n=== TOSS ===\n";
        std::cout << winner->getName() << " won the toss and elected to " << decision << " first.\n\n";
    }

    /**
     * @brief Start a new innings
     */
    void startInning(std::shared_ptr<Team> battingTeam, std::shared_ptr<Team> bowlingTeam) {
        currentInningNumber_++;
        
        // Reset score cards for both teams
        battingTeam->resetAllScoreCards();
        
        currentInning_ = std::make_shared<InningDetails>(
            currentInningNumber_, battingTeam, bowlingTeam, matchType_->getOversPerInning());
        
        innings_.push_back(currentInning_);
        
        // Set target for second innings
        if (currentInningNumber_ == 2 && !innings_.empty()) {
            currentInning_->setTargetScore(innings_[0]->getTotalRuns() + 1);
        }
        
        std::cout << "\n=== INNINGS " << currentInningNumber_ << " ===\n";
        std::cout << battingTeam->getName() << " batting\n";
        std::cout << bowlingTeam->getName() << " bowling\n\n";
    }

    /**
     * @brief Play a ball
     */
    void playBall(BallType ballType, int runs, RunType runType,
                  std::shared_ptr<Wicket> wicket = nullptr) {
        if (!currentInning_) {
            std::cout << "No active innings!\n";
            return;
        }
        
        auto striker = currentInning_->getStriker();
        auto nonStriker = currentInning_->getNonStriker();
        auto bowler = currentInning_->getCurrentBowler();
        
        int ballNumber = currentInning_->getBallsInCurrentOver() + 1;
        
        auto ballDetails = std::make_shared<BallDetails>(
            ballNumber, ballType, runs, runType, bowler, striker, nonStriker);
        
        if (wicket) {
            ballDetails->setWicket(wicket);
        }
        
        // Notify all observers to update scores
        notifyObservers(ballDetails);
        
        // Update innings
        bool isExtra = (runType == RunType::WIDE || runType == RunType::NO_BALL ||
                        runType == RunType::BYE || runType == RunType::LEG_BYE);
        currentInning_->addRuns(runs, runType, isExtra);
        
        if (wicket) {
            currentInning_->recordWicket(wicket);
        }
        
        // Check if over is complete
        if (ballDetails->isLegalDelivery() && 
            currentInning_->getBallsInCurrentOver() >= 6) {
            currentInning_->completeOver();
        }
        
        // Print ball summary
        std::cout << "[" << currentInning_->getOversString() << "] " 
                  << ballDetails->toString() << "\n";
        std::cout << "Score: " << currentInning_->getScoreSummary() << "\n\n";
    }

    /**
     * @brief Start a new over with specified bowler
     */
    void startOver(std::shared_ptr<Player> bowler) {
        if (!currentInning_) {
            std::cout << "No active innings!\n";
            return;
        }
        
        currentInning_->setCurrentBowler(bowler);
        std::cout << "\n--- New Over: " << bowler->getName() << " bowling ---\n\n";
    }

    /**
     * @brief End current innings
     */
    void endInning() {
        if (currentInning_) {
            std::cout << "\n" << currentInning_->getFullScoreBoard() << "\n";
        }
    }

    /**
     * @brief Determine match result
     */
    void determineResult() {
        if (innings_.size() < 2) {
            result_ = MatchResult::IN_PROGRESS;
            return;
        }
        
        int team1Score = innings_[0]->getTotalRuns();
        int team2Score = innings_[1]->getTotalRuns();
        int team2Wickets = innings_[1]->getTotalWickets();
        
        auto battingFirst = innings_[0]->getBattingTeam();
        auto battingSecond = innings_[1]->getBattingTeam();
        
        if (team2Score > team1Score) {
            result_ = (battingSecond == team1_) ? MatchResult::TEAM1_WON : MatchResult::TEAM2_WON;
            winner_ = battingSecond;
            int wicketsRemaining = 10 - team2Wickets;
            std::cout << "\n=== MATCH RESULT ===\n";
            std::cout << winner_->getName() << " won by " << wicketsRemaining << " wickets!\n";
        } else if (team1Score > team2Score) {
            result_ = (battingFirst == team1_) ? MatchResult::TEAM1_WON : MatchResult::TEAM2_WON;
            winner_ = battingFirst;
            int runDifference = team1Score - team2Score;
            std::cout << "\n=== MATCH RESULT ===\n";
            std::cout << winner_->getName() << " won by " << runDifference << " runs!\n";
        } else {
            result_ = MatchResult::TIE;
            std::cout << "\n=== MATCH RESULT ===\n";
            std::cout << "Match Tied!\n";
        }
    }

    std::string toString() const {
        std::string str = "╔══════════════════════════════════════════════════════╗\n";
        str += "║ Match: " + team1_->getName() + " vs " + team2_->getName() + "\n";
        str += "║ Format: " + matchType_->getMatchTypeName() + "\n";
        str += "║ Venue: " + venue_ + "\n";
        str += "║ Date: " + matchDate_ + "\n";
        str += "║ Status: " + matchResultToString(result_) + "\n";
        str += "╚══════════════════════════════════════════════════════╝\n";
        return str;
    }
};

} // namespace Cricbuzz
