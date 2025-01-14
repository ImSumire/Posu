#pragma once

#include <math.h>

#include <raylib.h>

#include <deque>
#include <vector>
#include <random>
#include <optional>


double random_angle() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<double> dis(0.0, 2.0 * M_PI);
    return dis(gen);
}

double dist(double x1, double y1, double x2, double y2) {
    return sqrtf64(powf64(x1 - x2, 2.0) + powf64(y1 - y2, 2.0));
}


struct Posu;


struct Circle {
    double spawn;

    double cosinus;
    double sinus;

    Circle(double dt) {
        spawn = dt;

        double angle = random_angle();
        cosinus = cos(angle);
        sinus = sin(angle);
    }

    /*
    bool update(Posu& posu) {
        double elapsed = posu.dt - spawn;
        if (elapsed > 2.0) {
            posu.missed += 1;
            return true;
        }

        double distance = powf64(elapsed * 12.0, 1.6);

        double x = posu.x + distance * 0.4 * cosinus;
        double y = posu.y + distance * 0.4 * sinus;

        double r = powf64(distance * 0.4, 0.7);

        if (posu.click && elapsed > 1.4) {
            auto m = GetMousePosition();

            if (dist(x, y, m.x, m.y) < r) {
                posu.catched += 1;
                posu.click = false;
                if (posu.click_sound.has_value()) {
                    PlaySound(posu.click_sound.value());
                }
                return true;
            }
        }

        return false;
    }
     */

    bool update(Posu& posu);
    void render(Posu& posu);

    /*
    void render(Posu& posu) {
        double elapsed = posu.dt - spawn;

        double distance = powf64(elapsed * 12.0, 1.6);

        double x = posu.x + distance * 0.4 * cosinus;
        double y = posu.y + distance * 0.4 * sinus;

        double r = powf64(distance * 0.4, 0.7);

        if (elapsed > 1.3) {
            DrawCircleLines(x, y, r, (Color) { 255, 255, 255, 255 });
        }
        
        else {
            DrawCircleLines(x, y, r, (Color) { 255, 255, 255, 64 });
        }
    }
     */
};


struct Posu {
    int x, y;

    // Sound system
    std::optional<Sound> click_sound = std::nullopt;

    // Update
    double dt;
    double last;
    double bpm = 0.0;
    bool click = false;
    std::deque<double> clicks;

    // Score
    int catched = 0;
    int missed = 0;

    // Circles
    std::vector<Circle> circles;

    Posu(int centerX, int centerY) {
        x = centerX;
        y = centerY;

        dt = last = GetTime();
    }

    ~Posu() {
        if (click_sound.has_value()) {
            UnloadSound(click_sound.value());
        }
    }

    void update() {
        dt = GetTime();

        // Detect click (spacebar or mouse button)
        if (
            IsKeyPressed(KEY_SPACE) ||
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
        ) {
            clicks.emplace_back(dt);
            click = true;
        }

        // Remove old clicks
        while (!clicks.empty() && dt - clicks.front() > 15.0) {
            clicks.pop_front();
        }

        // Get average interval
        double sum = 0.0;

        for (int i = 1; i < clicks.size(); i++) {
            sum += clicks[i] - clicks[i - 1];
        }
        double avg = sum / (clicks.size() - 1);

        if (avg > 0.01 && dt > last + avg) {
            circles.push_back(Circle(dt));
            last = dt;
        }

        bpm = 60.0 / avg;

        // Update circles
        for (int i = 0; i < circles.size(); i++) {
            if (circles.at(i).update(*this)) {
                circles.erase(circles.begin() + i);
            }
        }
    }

    void render() {
        for (auto& circle : circles) {
            circle.render(*this);
        }
    }
};

bool Circle::update(Posu& posu) {
    double elapsed = posu.dt - spawn;
    if (elapsed > 2.0) {
        posu.missed += 1;
        return true;
    }

    double distance = powf64(elapsed * 12.0, 1.6);

    double x = posu.x + distance * 0.4 * cosinus;
    double y = posu.y + distance * 0.4 * sinus;

    double r = powf64(distance * 0.4, 0.7);

    if (posu.click && elapsed > 1.4) {
        auto m = GetMousePosition();

        if (dist(x, y, m.x, m.y) < r) {
            posu.catched += 1;
            posu.click = false;
            if (posu.click_sound.has_value()) {
                PlaySound(posu.click_sound.value());
            }
            return true;
        }
    }

    return false;
}

void Circle::render(Posu& posu) {
    double elapsed = posu.dt - spawn;

    double distance = powf64(elapsed * 12.0, 1.6);

    double x = posu.x + distance * 0.4 * cosinus;
    double y = posu.y + distance * 0.4 * sinus;

    double r = powf64(distance * 0.4, 0.7);

    // DrawCircle(x, y, r, BLACK);

    if (elapsed > 1.3) {
        DrawCircleLines(
            x,
            y,
            r,
            (Color) { 255, 255, 255, 255 }
        );
    }
        
    else {
        DrawCircleLines(
            x,
            y,
            r,
            (Color) { 255, 255, 255, 64 }
        );
    }
}
