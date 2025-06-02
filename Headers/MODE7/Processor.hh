#ifndef MODE7_PROCESSOR_HH_
# define MODE7_PROCESSOR_HH_           202506L

# include <MODE7/Object.hh>

namespace MODE7
{
	/* a Processor abstracts the mechanics of selecting and writing to a
	 * a particular frame
	 *
	 * 
	 */
	class Processor: public Object
	{
		/* Types */
		public:
			typedef enum
			{
				Raw,             /* No processing except stripping to 7-bit */
				Teletext,        /* MODE 7-style 8-bit Teletext with CR/NL handling */
#if 0
				Teletext7,       /* 7-bit (escaped) Teletext */
				TTI,             /* TTI format (non-conformant lines are ignored) */
				Videotex         /* Videotex PLDS - ETSI 300 072 - i.e., Viewdata */
#endif
			} Kind;
		/* Class methods */
		public:
			static Processor *newProcessorOfKind(Kind kind, Carousel *carousel);
		
		/* Constructors and destructor */
		protected:
			Processor(Kind kind, Carousel *carousel);
			virtual ~Processor();
		
		/* Instance methods */
		public:
			virtual Processor *clone(void);
			virtual void setFrame(Frame *frame);
			virtual void putOctet(uint8_t octet) = 0;
			virtual void putOctets(const uint8_t *octet, size_t count);
			virtual void putch(int ch);
			virtual void puts(const char *str);
			virtual void clear(void);
			virtual void gotoxy(int col, int row);
		protected:
			virtual Frame *currentFrame(void);
			virtual Frame *defaultFrame(void);
		/* Instance data */
		private:
			Kind kind_;
			Carousel *carousel_;
			Frame *frame_;
		/* Inline accessors */
		public:
			inline Kind kind(void) const { return kind_; }
			inline Carousel *carousel(void) const { return carousel_; }
	};
}

#endif /*!MODE7_PROCESSOR_HH_*/
