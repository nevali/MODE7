#ifndef MODE7_CAROUSEL_HH_
# define MODE7_CAROUSEL_HH_            202506L

# include <cstdint>
# include <cstddef>

# include <MODE7/Constants.h>
# include <MODE7/Object.hh>

namespace MODE7
{
	class Magazine;
	class Page;
	class Frame;
	class Cursor;

	class Carousel: public Object
	{
		friend class Cursor;

		/* Class methods */
		public:
			static Carousel *newCarousel(void);

		/* Constructors and destructor */
		protected:
			Carousel();
			virtual ~Carousel();
		
		/* Instance methods */
		public:
			void setTeletextHeader(const char *string);
			void copyTeletextHeader(uint8_t *dest, size_t destsize);

			/* magazine numbered 0..7 where 0 is magazine #8 */
			Magazine *magazineNumbered(int number, bool create = true);
			/* high byte is magazine number, low byte is page number (0x606 = page 606, 0x0fc = page 8fc) */
			Page *pageNumbered(int number, bool create = true);
			/* subpage number uses the page number (immediately above) shifted left 8 bits */
			Frame *subpageNumbered(int number, bool create = true);
		
			/* perform any periodic refresh tasks */
			void refresh(void);
			
			/* obtain a means to iterate the Carousel's contents */
			Cursor *newCursor(void);
		
		/* Instance data */
		private:
			Magazine *magazines_[MODE7_MAGAZINES];
			uint8_t teletextHeader_[MODE7_HEADER];
	};
}

#endif /*!MODE7_CAROUSEL_HH_*/