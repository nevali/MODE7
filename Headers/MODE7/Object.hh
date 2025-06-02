#ifndef MODE7_OBJECT_HH_
# define MODE7_OBJECT_HH_              202506L

namespace MODE7
{
	/* A basic manually-reference-counted object used as a superclass of
	 * everything else.
	 *
	 * NOT thread-safe in the current implementation
	 */
	class Object
	{
		/* Types */
		public:
			typedef unsigned long RefCount;
		/* Constructors and destructor */
		protected:
			Object(void);
			virtual ~Object();

		/* Instance methods */
		public:
			virtual RefCount retain(void);
			virtual RefCount release(void);
		
		/* Instance data */
		private:
			RefCount refCount_;
	};
}

#endif /*!MODE7_OBJECT_HH_*/