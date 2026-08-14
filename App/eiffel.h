#pragma once

#include <cstdlib>
#include <glut.h>
#include <cmath>
#include <vector>

// Simple structure for 3D points and vectors
struct Vec3 { float x, y, z; };

class EiffelTower {

private:
    GLuint textureId = 0; // ID of the texture to apply

    // -------------------------------------------------------
    // Helper: Calculates the normal vector for a face
    // -------------------------------------------------------
    Vec3 CalculateNormal(Vec3 v1, Vec3 v2, Vec3 v3) {
        Vec3 u = { v2.x - v1.x, v2.y - v1.y, v2.z - v1.z };
        Vec3 v = { v3.x - v1.x, v3.y - v1.y, v3.z - v1.z };
        Vec3 n = { (u.y * v.z - u.z * v.y), (u.z * v.x - u.x * v.z), (u.x * v.y - u.y * v.x) };
        float len = sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
        // Handle zero length case to avoid division by zero
        if (len == 0.0f) return { 0.0f, 1.0f, 0.0f };
        return { n.x / len, n.y / len, n.z / len };
    }

    // -------------------------------------------------------
    // Helper: Draws a single textured quad
    // -------------------------------------------------------
    void DrawQuad(Vec3 v1, Vec3 v2, Vec3 v3, Vec3 v4, float v_bottom, float v_top) {
        Vec3 n = CalculateNormal(v1, v2, v3);
        glNormal3f(n.x, n.y, n.z);

        // Standard texture coordinates (u: 0.0 to 1.0, v: v_bottom to v_top)
        glTexCoord2f(0.0f, v_bottom); glVertex3f(v1.x, v1.y, v1.z);
        glTexCoord2f(1.0f, v_bottom); glVertex3f(v2.x, v2.y, v2.z);
        glTexCoord2f(1.0f, v_top);    glVertex3f(v3.x, v3.y, v3.z);
        glTexCoord2f(0.0f, v_top);    glVertex3f(v4.x, v4.y, v4.z);
    }

    // -------------------------------------------------------
    // Helper: Draws a straight, textured, tapered segment of a leg
    // This replaces the complex and bent DrawLegSegment
    // -------------------------------------------------------
    void DrawStraightLegSegment(Vec3 center_bottom, float w_bottom, Vec3 center_top, float w_top, float v_start, float v_end) {
        // Assume legs are mostly vertical, so we use simpler geometry
        float b = w_bottom / 2.0f;
        float t = w_top / 2.0f;
        float y_b = center_bottom.y;
        float y_t = center_top.y;
        float cx_b = center_bottom.x;
        float cz_b = center_bottom.z;
        float cx_t = center_top.x;
        float cz_t = center_top.z;

        // Vertices at bottom (y=y_b)
        Vec3 c1_bl = { cx_b - b, y_b, cz_b + b };
        Vec3 c1_br = { cx_b + b, y_b, cz_b + b };
        Vec3 c1_tr = { cx_b + b, y_b, cz_b - b };
        Vec3 c1_tl = { cx_b - b, y_b, cz_b - b };

        // Vertices at top (y=y_t)
        Vec3 c2_bl = { cx_t - t, y_t, cz_t + t };
        Vec3 c2_br = { cx_t + t, y_t, cz_t + t };
        Vec3 c2_tr = { cx_t + t, y_t, cz_t - t };
        Vec3 c2_tl = { cx_t - t, y_t, cz_t - t };

        glBegin(GL_QUADS);
        // Side 1 (Positive Z face)
        DrawQuad(c1_bl, c1_br, c2_br, c2_bl, v_start, v_end);
        // Side 2 (Positive X face)
        DrawQuad(c1_br, c1_tr, c2_tr, c2_br, v_start, v_end); // Note: Switched to c1_br, c1_tr, etc., assuming texture wrap
        // Side 3 (Negative Z face)
        DrawQuad(c1_tr, c1_tl, c2_tl, c2_tr, v_start, v_end);
        // Side 4 (Negative X face)
        DrawQuad(c1_tl, c1_bl, c2_bl, c2_tl, v_start, v_end);
        glEnd();
    }


    // -------------------------------------------------------
    // Draws the Realistic Arched Base (Now Straight Legs)
    // -------------------------------------------------------
    void DrawRealisticBase(float baseOffset, float topOffset, float h) {
        // Significantly increased leg thickness for a massive look
        float legW_base = 1.6f;
        float legW_mid1 = 1.3f;
        float legW_mid2 = 1.0f;
        float legW_top = 0.8f;

        // Define Center Points (P0 is on the ground, P3 is at height 'h')
        // X and Z coordinates are kept constant across segments to ensure straightness
        // The width difference provides the taper.

        // Initial point (Ground level, outer offset)
        Vec3 p0 = { baseOffset, 0.0f, baseOffset };
        // Mid-point 1 (1/3 height, slightly closer to center)
        Vec3 p1 = { baseOffset * 0.95f, h * 0.33f, baseOffset * 0.95f };
        // Mid-point 2 (2/3 height, closer to center)
        Vec3 p2 = { baseOffset * 0.8f, h * 0.66f, baseOffset * 0.8f };
        // Final point (Top of base, inner offset)
        Vec3 p3 = { topOffset, h, topOffset };

        float signs[4][2] = { {1,1}, {-1,1}, {-1,-1}, {1,-1} };

        for (int i = 0; i < 4; i++) {
            float sx = signs[i][0];
            float sz = signs[i][1];

            // Apply signs to the center points for each of the four legs
            Vec3 cp0 = { p0.x * sx, p0.y, p0.z * sz };
            Vec3 cp1 = { p1.x * sx, p1.y, p1.z * sz };
            Vec3 cp2 = { p2.x * sx, p2.y, p2.z * sz };
            Vec3 cp3 = { p3.x * sx, p3.y, p3.z * sz };

            // Draw the three straight segments
            DrawStraightLegSegment(cp0, legW_base, cp1, legW_mid1, 0.0f, 0.33f);
            DrawStraightLegSegment(cp1, legW_mid1, cp2, legW_mid2, 0.33f, 0.66f);
            DrawStraightLegSegment(cp2, legW_mid2, cp3, legW_top, 0.66f, 1.0f);

            // Draw the Platform (Foot) - now textured and slightly repositioned
            glPushMatrix();
            // Centered under the base of the leg
            glTranslatef(cp0.x, -0.01f, cp0.z); // Moved slightly down so it sits on y=0
            DrawPlatform(legW_base + 0.6f, 0.1f); // Less tall foot
            glPopMatrix();
        }
    }

    // -------------------------------------------------------
    // Helper: Draws a textured frustum (tapered box)
    // Puts base at y=0, top at y=height.
    // -------------------------------------------------------
    void DrawSolidFrustum(float baseW, float topW, float height) {
        float b = baseW / 2.0f; float t = topW / 2.0f;
        // Base vertices (y=0)
        Vec3 v_bl = { -b, 0, b }; Vec3 v_br = { b, 0, b }; Vec3 v_tr = { b, 0, -b }; Vec3 v_tl = { -b, 0, -b };
        // Top vertices (y=height)
        Vec3 v_t_bl = { -t, height, t }; Vec3 v_t_br = { t, height, t }; Vec3 v_t_tr = { t, height, -t }; Vec3 v_t_tl = { -t, height, -t };

        glBegin(GL_QUADS);
        DrawQuad(v_bl, v_br, v_t_br, v_t_bl, 0.0f, 1.0f); // Front (+Z)
        DrawQuad(v_br, v_tr, v_t_tr, v_t_br, 0.0f, 1.0f); // Right (+X)
        DrawQuad(v_tr, v_tl, v_t_tl, v_t_tr, 0.0f, 1.0f); // Back (-Z)
        DrawQuad(v_tl, v_bl, v_t_bl, v_t_tl, 0.0f, 1.0f); // Left (-X)
        glEnd();
    }

    // -------------------------------------------------------
    // Helper: Draws a textured platform (flat box)
    // Centered at (0,0,0), extends from -height/2 to +height/2.
    // -------------------------------------------------------
    void DrawPlatform(float width, float height) {
        float w = width / 2.0f; float h = height / 2.0f;
        Vec3 v1 = { -w, -h, w }; Vec3 v2 = { w, -h, w }; Vec3 v3 = { w, h, w }; Vec3 v4 = { -w, h, w };
        Vec3 v5 = { -w, -h, -w }; Vec3 v6 = { w, -h, -w }; Vec3 v7 = { w, h, -w }; Vec3 v8 = { -w, h, -w };

        glBegin(GL_QUADS);
        DrawQuad(v1, v2, v3, v4, 0, 1); // Front (+Z)
        DrawQuad(v2, v6, v7, v3, 0, 1); // Right (+X)
        DrawQuad(v6, v5, v8, v7, 0, 1); // Back (-Z)
        DrawQuad(v5, v1, v4, v8, 0, 1); // Left (-X)

      
        glNormal3f(0.0f, 1.0f, 0.0f); // Top normal
        glTexCoord2f(0.0f, 0.0f); glVertex3f(v4.x, v4.y, v4.z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(v3.x, v3.y, v3.z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(v7.x, v7.y, v7.z);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(v8.x, v8.y, v8.z);

        glNormal3f(0.0f, -1.0f, 0.0f); // Bottom normal
        glTexCoord2f(0.0f, 0.0f); glVertex3f(v5.x, v5.y, v5.z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(v6.x, v6.y, v6.z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(v2.x, v2.y, v2.z);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(v1.x, v1.y, v1.z);
        glEnd();
    }


public:
    void SetTexture(GLuint id) {
        textureId = id;
    }

    void Draw(float x, float y, float z, float scale) {
        glPushMatrix();
        glTranslatef(x, y, z);
        glScalef(scale, scale, scale);

        if (textureId != 0) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, textureId);
            glColor3f(1.0f, 1.0f, 1.0f); // Ensure color is white for full texture visibility
        }
        else {
            GLfloat mat_ambient[] = { 0.1f, 0.15f, 0.3f, 1.0f };
            GLfloat mat_diffuse[] = { 0.25f, 0.4f, 0.8f, 1.0f };
            glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        }

        // --- Draw the Tower ---
        float currentY = 0.0f;

        // 1. Arched Base (Now with straight, tapered legs and textured feet)
        float baseH = 5.0f;
        DrawRealisticBase(4.0f, 2.5f, baseH);
        currentY += baseH;

        // 2. First Platform
        float p1H = 0.6f;
        glPushMatrix();
        glTranslatef(0.0f, currentY + p1H / 2.0f, 0.0f);
        DrawPlatform(5.8f, p1H);
        glPopMatrix();
        currentY += p1H;

        // 3. Middle Section
        float midH = 9.0f;
        glPushMatrix();
        glTranslatef(0.0f, currentY, 0.0f);
        DrawSolidFrustum(5.2f, 3.0f, midH);
        glPopMatrix();
        currentY += midH;

        // 4. Second Platform
        float p2H = 0.6f;
        glPushMatrix();
        glTranslatef(0.0f, currentY + p2H / 2.0f, 0.0f);
        DrawPlatform(3.8f, p2H);
        glPopMatrix();
        currentY += p2H;

        // 5. Top Section (Main Spire Body)
        float topH = 11.0f;
        glPushMatrix();
        glTranslatef(0.0f, currentY, 0.0f);
        DrawSolidFrustum(3.0f, 1.6f, topH);
        glPopMatrix();
        currentY += topH;

        // --- NEW TOP STRUCTURE ---

        // 6. The thicker "Observation Deck" block
        float deckH = 1.2f;
        glPushMatrix();
        glTranslatef(0.0f, currentY + deckH / 2.0f, 0.0f);
        DrawPlatform(2.0f, deckH);
        glPopMatrix();
        currentY += deckH;

        // 7. The narrower "Lantern" section
        float lanternH = 2.5f;
        glPushMatrix();
        glTranslatef(0.0f, currentY, 0.0f);
        DrawSolidFrustum(1.5f, 1.0f, lanternH);
        glPopMatrix();
        currentY += lanternH;

        // 8. The Final Thin Antenna Mast
        float antennaH = 5.0f;
        glPushMatrix();
        glTranslatef(0.0f, currentY + antennaH / 2.0f, 0.0f);
        DrawPlatform(0.3f, antennaH);
        glPopMatrix();
        // -------------------------------------------

        if (textureId != 0) {
            glDisable(GL_TEXTURE_2D);
        }

        glPopMatrix();
    }
};