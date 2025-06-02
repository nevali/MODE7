#ifndef MODE7_PAGE_HH_
# define MODE7_PAGE_HH_                202506L

# include <cstdint>

# include <MODE7/Object.hh>
# include <MODE7/Magazine.hh>

namespace MODE7
{
	class Frame;

	class Page: public Object
	{
		/* Constructors and destructor */
		public:
			Page(Magazine *mag, uint8_t pageNumber); /* 00..ff (not including magazine digit) */
		protected:
			virtual ~Page();
		
		/* Instance methods */
		public:
			uint8_t number(void) const; /* page number within magazine */
			int pageNumber(void) const; /* page number prefixed with magazine number */
			Frame *frameNumbered(uint8_t number, bool create = true); /* obtain subpage numbered 0..255 */
			uint8_t frameCount(void); /* number of defined frames */
			void reset(void); /* delete all frames */
			Frame *activeFrame(void); /* return the active frame of this page */
			bool setActiveFrame(uint8_t number); /* change the active frame of this page */
			void cycle(void); /* cycle to the next frame, if the cycle time has been reached */
		
		/* Instance data */
		private:
			Magazine *magazine_;
			uint8_t number_;
			int pageNum_;
			Frame **frames_;
			size_t frameCount_;
		
		/* Inline accessors */
		public:
			inline Magazine *magazine(void) const { return magazine_; }
	};
}

#endif /*!MODE7_PAGE_HH_*/
