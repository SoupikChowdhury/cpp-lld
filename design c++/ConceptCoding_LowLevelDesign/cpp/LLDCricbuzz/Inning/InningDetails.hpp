#pragma once

#include <vector>
#include <memory>
#include <string>
#include "OverDetails.hpp"
#include "../Team/Team.hpp"
#include "../Team/Wicket.hpp"

namespace Cricbuzz {

/**
 * @brief Class representing details of an innings
 */
class InningDetails {
private:
    int inningNumber_;
    std::shared_ptr<Team> battingTeam_;
    std::shared_ptr<Team> bowlingTeam_;
    std::vector<std::shared_ptr<OverDetails>> overs_;
    std::vector<std::shared_ptr<Wicket>> wickets_;
    
    int totalRuns_ = 0;
    int totalWickets_ = 0;
    int totalExtras_ = 0;
    int wides_ = 0;
    int noBalls_ = 0;
    int byes_ = 0;
    int legByes_ = 0;
    
    int currentBatsmanIndex_ = 0;
    int nextBatsmanIndex_ = 1;
    std::shared_ptr<Player> striker_;
    std::shared_ptr<Player> nonStriker_;
    std::shared_ptr<Player> currentBowler_;
    std::shared_ptr<OverDetails> currentOver_;
    
    int maxOvers_;
    bool isComplete_ = false;
    int targetScore_ = 0;  // For second innings

public:
    InningDetails(int inningNumber, std::shared_ptr<Team> battingTeam,
                  std::shared_ptr<Team> bowlingTeam, int maxOvers)
        : inningNumber_(inningNumber)
        , battingTeam_(battingTeam)
        , bowlingTeam_(bowlingTeam)
        , maxOvers_(maxOvers) {
        
        // Initialize first two batsmen
        if (battingTeam_->getTeamSize() >= 2) {
            striker_ = battingTeam_->getPlayer(0);
            nonStriker_ = battingTeam_->getPlayer(1);
            nextBatsmanIndex_ = 2;
        }
    }

    // Getters
    int getInningNumber() const { return inningNumber_; }
    std::shared_ptr<Team> getBattingTeam() const { return battingTeam_; }
    std::shared_ptr<Team> getBowlingTeam() const { return bowlingTeam_; }
    int getTotalRuns() const { return totalRuns_; }
    int getTotalWickets() const { return totalWickets_; }
    int getTotalExtras() const { return totalExtras_; }
    std::shared_ptr<Player> getStriker() const { return striker_; }
    std::shared_ptr<Player> getNonStriker() const { return nonStriker_; }
    std::shared_ptr<Player> getCurrentBowler() const { return currentBowler_; }
    bool isComplete() const { return isComplete_; }
    int getMaxOvers() const { return maxOvers_; }
    const std::vector<std::shared_ptr<OverDetails>>& getOvers() const { return overs_; }

    // Setters
    void setTargetScore(int target) { targetScore_ = target; }
    void setCurrentBowler(std::shared_ptr<Player> bowler) {
        currentBowler_ = bowler;
        currentOver_ = std::make_shared<OverDetails>(overs_.size(), bowler);
    }

    /**
     * @brief Get current over number
     */
    int getCurrentOverNumber() const { return overs_.size(); }

    /**
     * @brief Get balls in current over
     */
    int getBallsInCurrentOver() const {
        return currentOver_ ? currentOver_->getLegalBallCount() : 0;
    }

    /**
     * @brief Get overs string (e.g., "15.3")
     */
    std::string getOversString() const {
        int completedOvers = overs_.size();
        int balls = currentOver_ ? currentOver_->getLegalBallCount() : 0;
        return std::to_string(completedOvers) + "." + std::to_string(balls);
    }

    /**
     * @brief Add runs to the innings
     */
    void addRuns(int runs, RunType runType, bool isExtra = false) {
        totalRuns_ += runs;
        
        if (isExtra) {
            totalExtras_ += runs;
            switch (runType) {
                case RunType::WIDE: wides_ += runs; break;
                case RunType::NO_BALL: noBalls_ += runs; break;
                case RunType::BYE: byes_ += runs; break;
                case RunType::LEG_BYE: legByes_ += runs; break;
                default: break;
            }
        }
        
        // Rotate strike for odd runs
        if (runs % 2 == 1) {
            rotateStrike();
        }
        
        // Check if target achieved (second innings)
        if (targetScore_ > 0 && totalRuns_ >= targetScore_) {
            isComplete_ = true;
        }
    }

    /**
     * @brief Record a wicket
     */
    void recordWicket(std::shared_ptr<Wicket> wicket) {
        wickets_.push_back(wicket);
        totalWickets_++;
        
        // Bring in next batsman
        if (nextBatsmanIndex_ < static_cast<int>(battingTeam_->getTeamSize())) {
            striker_ = battingTeam_->getPlayer(nextBatsmanIndex_);
            nextBatsmanIndex_++;
        } else {
            isComplete_ = true;  // All out
        }
    }

    /**
     * @brief Complete current over
     */
    void completeOver() {
        if (currentOver_) {
            overs_.push_back(currentOver_);
            currentOver_ = nullptr;
            rotateStrike();  // Change of ends
            
            if (static_cast<int>(overs_.size()) >= maxOvers_) {
                isComplete_ = true;
            }
        }
    }

    /**
     * @brief Rotate strike between batsmen
     */
    void rotateStrike() {
        std::swap(striker_, nonStriker_);
    }

    /**
     * @brief Get current run rate
     */
    double getRunRate() const {
        int totalBalls = overs_.size() * 6 + getBallsInCurrentOver();
        if (totalBalls == 0) return 0.0;
        return (static_cast<double>(totalRuns_) / totalBalls) * 6.0;
    }

    /**
     * @brief Get required run rate (for chasing team)
     */
    double getRequiredRunRate() const {
        if (targetScore_ == 0) return 0.0;
        int runsNeeded = targetScore_ - totalRuns_;
        int ballsRemaining = (maxOvers_ * 6) - (overs_.size() * 6 + getBallsInCurrentOver());
        if (ballsRemaining <= 0) return 0.0;
        return (static_cast<double>(runsNeeded) / ballsRemaining) * 6.0;
    }

    std::string getScoreSummary() const {
        std::string str = battingTeam_->getName() + ": ";
        str += std::to_string(totalRuns_) + "/" + std::to_string(totalWickets_);
        str += " (" + getOversString() + " ov)";
        str += " | RR: " + std::to_string(getRunRate()).substr(0, 4);
        
        if (targetScore_ > 0) {
            str += " | Need " + std::to_string(targetScore_ - totalRuns_) + " runs";
            str += " | RRR: " + std::to_string(getRequiredRunRate()).substr(0, 4);
        }
        
        return str;
    }

    std::string getFullScoreBoard() const {
        std::string str = "========================================\n";
        str += "         " + battingTeam_->getName() + " INNINGS\n";
        str += "========================================\n\n";
        
        str += battingTeam_->getScoreBoard() + "\n";
        
        str += "Extras: " + std::to_string(totalExtras_);
        str += " (Wd: " + std::to_string(wides_);
        str += ", Nb: " + std::to_string(noBalls_);
        str += ", B: " + std::to_string(byes_);
        str += ", Lb: " + std::to_string(legByes_) + ")\n\n";
        
        str += "TOTAL: " + std::to_string(totalRuns_) + "/" + std::to_string(totalWickets_);
        str += " (" + getOversString() + " overs)\n\n";
        
        str += bowlingTeam_->getBowlingFigures();
        
        return str;
    }
};

} // namespace Cricbuzz
