#ifndef MODE7_MAGAZINE_HH_
# define MODE7_MAGAZINE_HH_            202506L

# include <cstdint>

# include <MODE7/Object.hh>

namespace MODE7
{
	class Page;
	class Carousel;

	class Magazine: public Object
	{
		friend class Carousel;

		/* Constructors and destructor */
		protected:
			Magazine(Carousel *carousel, int number); /* 1..8 */
			virtual ~Magazine();
		
		/* Instance methods */
		public:
			Page *pageNumbered(uint8_t number, bool create = true);
		
		/* Instance data */
		private:
			Carousel *carousel_;
			int number_;
			Page **pages_;
			size_t pageCount_;
		
		/* Inline accessors */
		public:
			inline int number(void) const { return (number_ ? number_ : 8); };
			inline Carousel *carousel(void) const { return carousel_; }
	};
}

#endif /*MODE7_MAGAZINE_HH_*/
