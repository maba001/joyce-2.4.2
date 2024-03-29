/************************************************************************

    JOYCE v2.1.0 - Amstrad PCW emulator

    Copyright (C) 1996, 2001-2  John Elliott <seasip.webmaster@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*************************************************************************/

class AnneTerminal : public PcwTerminal
{
public:
	AnneTerminal(PcwBeeper *b);
	virtual ~AnneTerminal();

	virtual void onGainFocus();
	virtual void onLoseFocus();
	virtual void onScreenTimer();

	virtual void out(zword port, zbyte value);
        virtual void setForeground(zbyte r, zbyte g, zbyte b);
        virtual void setBackground(zbyte r, zbyte g, zbyte b);
	virtual void onDebugSwitch(bool debugMode);
	inline Uint8 getRefresh()        { return m_scrRefresh; }
	inline void  setRefresh(Uint8 r) { m_scrRefresh = r; }

	bool loadBitmaps(void);
	void border(zbyte b);
	void enable(bool b = true);
	void powerLED(bool green);
        virtual void reset(void);
//
// XXX Native output (not yet implemented)
//
        virtual PcwTerminal &operator << (unsigned char c);
//
// Native acceleration
//
        virtual bool doBlit(unsigned xfrom, unsigned yfrom, unsigned w,
                                unsigned h, unsigned xto, unsigned yto);
        virtual bool doRect(unsigned x, unsigned y, unsigned w, unsigned h,
                                zbyte fill, zbyte rasterOp, zbyte *pattern = NULL);
	virtual bool doBitmap(unsigned x, unsigned y, unsigned w, unsigned h,
                                zbyte *data, zbyte *mask = NULL, int slice = 0);

protected:
	virtual bool drawBitmap(unsigned x, unsigned y, unsigned w, unsigned h,
                                zbyte *data, zbyte *mask, unsigned swb);
	void drawPcwScr(void);
	void drawPower(void);

	int  m_monochrome;
	int  m_indices[32];		// Colour indices for the 32 colours
					// the screen can display
	SDL_Colour m_inks[16];		// The current colours in the 16 
					// palette registers
	zbyte    m_palette[16];		// The bytes in the 16 palette registers, 0-32
	int	m_pcwDInk[16];		// The indexes of the 16 colours in use
	int	m_pcwInk[16];		// Their actual indexes after inversion
	SDL_Colour m_beepFg, m_beepBg;

//
// Variables for drawing the power switch
//
	SDL_Surface *m_bmpR, *m_bmpG;
	bool m_power, m_powerDrawn;
//
// Variables for drawing PCW video RAM
//
	bool	m_borderInvalid;	// Border needs redraw?
	int	m_visible;		// Screen visible?
	int	m_beepClr;		// Colour for border beep
	int	m_scrEnabled;		// 0 : Screen is border only
					// 1 : Screen is normal terminal
					// 2 : Screen is border only, but
					//    needs to be zapped.
	Uint8	m_colourReg;		// Colour register.
					// Bit 6 set if screen is visible.
	Uint8   m_prevColourReg;	// Previous value of colour register
	Uint8	m_scrOrigin;		// Vertical origin
	Uint8	*m_rollerRam;		// PCW R-RAM
//
// Refresh settings
//
	int	m_scrCount, m_scrRefresh;
//
// XML settings
//
	bool parseNode(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur);
	bool storeNode(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur);
//
// Settings functions.
// See if this device has user-settable options. If it does, populates
// "key" and "caption" and returns true; else, returns false.
//
public:
        virtual bool hasSettings(SDLKey *key, string &caption);
//
// Display settings screen
// Return 0 if OK, -1 if quit message received.
// 
        virtual UiEvent settings(UiDrawer *d);
private:
	UiEvent customClrs(UiDrawer *d);

//
// Beeper support
//
public:
	virtual void beepOn(void);
	virtual void beepOff(void);
	void fillerup(Uint8 *stream, int len);
	void initSound(void);
	void deinitSound(void);
        virtual void tick(void);

protected:
	BeeperMode m_beepMode;
	PcwBeeper *m_beeper;
};





