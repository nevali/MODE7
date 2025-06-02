#ifndef MODE7_FRAME_HH_
# define MODE7_FRAME_HH_               202506L

# include <MODE7/Constants.h>
# include <MODE7/Object.hh>
# include <MODE7/Page.hh>

namespace MODE7
{
	/* A Frame (subpage) is the entity that actually buffers Teletext
	 * output to bestreamed as packets - it essentially provides a 40x24
	 * drawable 'canvas'
	 *
	 * character coordinates are numbered from (1,1) in the top-left corner
	 * to (40,24) in the bottom-right.
	 */
	class Frame: public Object
	{
		/* Constructors and destructor */
		public:
			Frame(Page *page, uint16_t number); /* subpages are numbered 0..3f7f */
		protected:
			virtual ~Frame();
		
		/* Instance methods */
		public:
			void clear(void); /* clear entire frame and reset position to (1,1) */
			void gotoLine(int line); /* 1..24 */
			void gotoxy(int x, int y); /* x = 1..80, y = 1..24 */
			void clearLine(void); /* clear current line and move x to position 1 */
			void nputch(const uint8_t octet); /* write a single character at current position without advancing cursor */
			void putch(const uint8_t octet); /* write a single character at current position */
			void write(const uint8_t *octets, size_t size); /* write octets at current position */
			void puts(const char *str); /* write null-terminated string at current position */
			void up(bool wrap = true); /* move up one row */
			void down(bool wrap = true); /* move down one row */
			void left(bool wrap = true); /* move left one position */
			void right(bool wrap = true); /* move right one position */
			void scrollup(int lines);
			void newline(void); /* move cursor to start of next line, scrolling up if necessary */

			const uint8_t *octetsAtRow(uint8_t row) const; /* return a pointer into the frame's character buffer */
		
		/* Instance data */
		private:
			Page *page_;
			uint16_t number_;
			uint8_t x_, y_;
			bool dirty_;
			uint8_t buffer_[MODE7_WIDTH * MODE7_TEXTROWS];
		
		/* Inline accessors */
		public:
			inline bool dirty(void) const { return dirty_; };
			inline uint16_t number(void) const { return number_; }
			inline Page *page(void) const { return page_; }
			inline int curx(void) const { return x_; }
			inline int cury(void) const { return y_; }
	};
}

#endif /*!MODE7_FRAME_HH_*/