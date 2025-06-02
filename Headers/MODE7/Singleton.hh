#ifndef MODE7_SINGLETON_HH_
# define MODE7_SINGLETON_HH_           202506L

# include "Object.hh"

namespace MODE7
{
	/* Once created, instances of Singleton are never destroyed
	 * To be used sparingly for obvious reasons
	 */
	class Singleton: public Object
	{
		protected:
			Singleton(void);
			virtual ~Singleton();
		/* Instance methods */
		public:
			virtual /*override*/ RefCount retain(void);
			virtual /*override*/ RefCount release(void);
	};
}

#endif /*!MODE7_SINGLETON_HH_*/
