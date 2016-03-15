// Copyright (C) 2012-2015 Leap Motion, Inc. All rights reserved.
#include "stdafx.h"
#include "auto_id.h"

// Index that will be given to the next auto_id instance.  Zero is reserved.
static int s_index = 1;

const autowiring::auto_id_block auto_id_t<void>::s_block{
  0,
  &typeid(void),
  &typeid(autowiring::s<void>),
  0,
  0,
  nullptr,
  nullptr
};

int autowiring::CreateIndex(void) {
  return s_index++;
}

std::shared_ptr<CoreObject> autowiring::auto_id_block::NullToObj(const std::shared_ptr<void>&)
{
  return nullptr;
}

std::shared_ptr<void> autowiring::auto_id_block::NullFromObj(const std::shared_ptr<CoreObject>&)
{
  return nullptr;
}
