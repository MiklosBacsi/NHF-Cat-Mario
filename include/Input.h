/** @file Input.h
  * @brief Stores the states of the keys.
  * @author Bácsi Miklós
  * @date 2025-05-18
*/

#ifndef INPUT_H
#define INPUT_H

class Input {
private:
    int MouseX; ///< Contains x coordinate of the mouse.
    int MouseY; ///< Contains y coordinate of the mouse.
    bool MouseClick;    ///< Contains whether the mouse is clicked.
    bool Esc;   ///< Contains key 'Esc' is pressed.
    bool W;     ///< Contains key 'W' is pressed.
    bool A;     ///< Contains key 'A' is pressed.
    bool S;     ///< Contains key 'S' is pressed.
    bool D;     ///< Contains key 'D' is pressed.
    bool P;     ///< Contains key 'P' is pressed.
    bool Space; ///< Contains key 'Space' is pressed.
    bool disableEsc;    ///< Contains whether the key 'Esc' is disabled (necessary for smooth input).
    bool disableP;      ///< Contains whether the key 'P' is disabled (necessary for smooth input).
public:
    /**
     * Constructor that initialises all boolean values to false.
     */
    Input();

    /**
     * @return Returns the x coordinate of the mouse.
     */
    int GetMouseX() const;

    /**
     * @return Returns the y coordinate of the mouse.
     */
    int GetMouseY() const;

    /**
     * @return Returns whether the mouse is pressed.
     */
    bool GetMouseClick() const;

    /**
     * @return Returns whether the key 'Esc' is pressed.
     */
    bool GetEsc() const;

    /**
     * @return Returns whether the up is pressed.
     */
    bool GetUp() const;

    /**
     * @return Returns whether the left is pressed.
     */
    bool GetLeft() const;

    /**
     * @return Returns whether the down is pressed.
     */
    bool GetDown() const;

    /**
     * @return Returns whether the right is pressed.
     */
    bool GetRight() const;

    /**
     * @return Returns whether the pause is pressed.
     */
    bool GetPause() const;

    /**
     * @return Returns whether the key 'Space' is pressed.
     */
    bool GetSpace() const;

    /**
     * @brief Sets the x coordinate of the mouse.
     */
    void SetMouseX(int x);

    /**
     * @brief Sets the y coordinate of the mouse.
     */
    void SetMouseY(int y);

    /**
     * @brief Sets the clicked state of the mouse.
     */
    void SetMouseClick(bool pressed);

    /**
     * @brief Sets the key 'Esc'.
     */
    void SetEsc(bool pressed);

    /**
     * @brief Sets the up state.
     */
    void SetW(bool pressed);

    /**
     * @brief Sets the left state.
     */
    void SetA(bool pressed);

    /**
     * @brief Sets the down state.
     */
    void SetS(bool pressed);

    /**
     * @brief Sets the right state.
     */
    void SetD(bool pressed);

    /**
     * @brief Sets the key 'P'.
     */
    void SetP(bool pressed);

    /**
     * @brief Sets the key 'Space'.
     */
    void SetSpace(bool pressed);

    /**
     * @return Returns reference to the disable Esc.
     */
    bool& DisableEsc();

    /**
     * @return Returns reference to the disable P.
     */
    bool& DisableP();

    /**
     * @brief Destructor.
     */
    ~Input();
};

#endif // INPUT_H