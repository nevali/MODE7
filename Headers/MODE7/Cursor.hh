#ifndef MODE7_CURSOR_HH_
# define MODE7_CURSOR_HH_              202506L

# include <MODE7/Object.hh>

namespace MODE7
{
	class Carousel;
	class CursorDelegate;

	class Cursor: public Object
	{
		friend class Carousel;

		/* Constructors and destructor */
		protected:
			Cursor(Carousel *carousel);
			~Cursor();
		
		/* Instance methods */
		public:
			/* Return the current cursor position */
			Magazine *magazine(void);
			Page *page(void);
			Frame *frame(void);

			/* advance the cursor to the next page; if the last page in
			 * the magazine, nextMagazine() is invoked automatically 
			 */
			bool nextPage(void);
			
			/* reset the cursor to the first page in the current magazine */
			bool firstPage(void);

			/* advance the cursor to the first page of the next magazine */
			bool nextMagazine(void);

		/* Instance data */
		private:
			uint8_t mseq_;
			uint8_t pseq_;
			Carousel *carousel_;
			Magazine *magazine_;
			Page *page_;
			Frame *frame_;
	};
}

#endif /*!MODE7_CURSOR_HH_*/
