#ifndef _DEPENDENT_CONTEXT_H
#define _DEPENDENT_CONTEXT_H

class CoreContext;

template<class T>
class DependentContext:
  public T
{
public:
  DependentContext(const std::shared_ptr<CoreContext>& context):
    m_context(context)
  {}

  std::shared_ptr<CoreContext> m_context;
};

#endif
